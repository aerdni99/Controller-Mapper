#include "MainComponent.h"

MainComponent::MainComponent() {
    //==============================================================================
    setSize(1000, 800);

    mainScreen = std::make_unique<MainContent>();
    mainScreen->onToggleConsole = [this]() {
        toggleConsole();
        };
    mainScreen->onShowControllerSelector = [this]() {
        showControllerSelector();
        };
    addAndMakeVisible(*mainScreen);

    midiConsole = std::make_unique<MidiConsoleComponent>();
    addAndMakeVisible(*midiConsole);

    resizerBar = std::make_unique<ComponentResizerBar>(&layoutManager, 1, false);
    addAndMakeVisible(*resizerBar);

    layoutManager.setItemLayout(0, -1.00, -1.00, -1.00);
    layoutManager.setItemLayout(1, -0.00, -0.00, -0.00);
    layoutManager.setItemLayout(2, -0.00, -0.00, -0.00);

    //==============================================================================
    // Start timer for polling midi messages
    startTimerHz(30);

    // Initialize GUI components
    resized();

    // Initialize Controller
    controllerSelector = std::make_unique<ControllerSelector>();
    controller = controllerSelector->getController();

    // Midi input (This looks for the virtual midi port called "Controller Mapper" and listens for inputs from that controller. Change this to "KeyLab mkII 61" To listen to my keyboard
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    if (midiInputs.isEmpty()) {
        DBG("No MIDI devices detected");
    } 
    else {
        for (auto& device : midiInputs) {
            if (device.name == "Controller Mapper") {
                midiInput = juce::MidiInput::openDevice(device.identifier, this);
                if (midiInput)
                    midiInput->start();
            }
        }
        
    }

    // Open MIDI output to virtual device
    juce::MidiDeviceInfo myDevice;
    for (auto& device : juce::MidiOutput::getAvailableDevices()) {
        if (device.name == "Controller Mapper") {
            myDevice = device;
            break;
        }
    }
    if (myDevice.identifier.isNotEmpty()) {
        midiOutput = juce::MidiOutput::openDevice(myDevice.identifier);
        if (midiOutput) {
            DBG("MIDI Output Opened: " << myDevice.name);
        }
    }
    else {
        DBG("Didn't find virtual midi port");
    }

    // I found this offset by checking the raw value of signals sent by my controller's joysticks and they were always sending a signal 0-3500, so thats the "deadzone"
    deadzoneOffset = 4000;

    // Scene 1 uses midi CC numbers 11-16. when the scene changes, this number will change based on the available sets of CC codes.
    sceneOffset = 11;

    DBG("Construction Complete!");
}

MainComponent::~MainComponent() {
}

//==============================================================================

void MainComponent::paint (juce::Graphics& g) {
    auto area = getLocalBounds().reduced(10);

    // (Our component is white
    g.fillAll(juce::Colours::lightgrey);
    g.setColour(juce::Colours::white);
    g.fillRect(area);
}

void MainComponent::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds().reduced(10);

    juce::Component* components[] = {
        mainScreen.get(),
        resizerBar.get(),
        midiConsole.get()
    };

    layoutManager.layOutComponents(
        components,
        3,
        10, 10,
        area.getWidth(), area.getHeight(),
        true, true
    );
}

void MainComponent::toggleConsole() {
    isConsoleVisible = !isConsoleVisible;

    if (isConsoleVisible) {
        midiConsole->setVisible(true);
        resizerBar->setVisible(true);
        layoutManager.setItemLayout(0, -0.69, -0.89, -0.79);
        layoutManager.setItemLayout(1, -0.01, -0.01, -0.01);
        layoutManager.setItemLayout(2, -0.10, -0.30, -0.20);
    }
    else {
        midiConsole->setVisible(false);
        resizerBar->setVisible(false);
        layoutManager.setItemLayout(0, -1.00, -1.00, -1.00);
        layoutManager.setItemLayout(1, -0.00, -0.00, -0.00);
        layoutManager.setItemLayout(2, -0.00, -0.00, -0.00);
    }
    resized();
}

// This function constructs MIDI messages and adds them to a string array object. 
void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
    if (!isConsoleVisible) return;

    juce::String msgText;

    if (message.isNoteOn()) {
        msgText = "Note On: " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3)
            + "Vel: " + juce::String(message.getVelocity());
    }
    else if (message.isNoteOff()) {
        msgText = "Note Off: " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3);
    }
    else if (message.isController()) {
        msgText = "Control Change: CC#" + juce::String(message.getControllerNumber()) + "Val: " + juce::String(message.getControllerValue());
    }
    else {
        msgText = message.getDescription();
    }

    midiLogQueue.add(msgText);
    return;
}

void MainComponent::timerCallback() { 
    // poll for SDL messages.
    SDLPolling();

    // log incoming midi messages to MY CONSOLE
    sendToConsole();
    return;
}

void MainComponent::showControllerSelector() {
    return;
}

int MainComponent::axisConversion(SDL_GamepadAxisEvent axisEvent) {
    //Trigger
    if (axisEvent.axis == 4 || axisEvent.axis == 5) {
        return axisEvent.value * 127.0f / 32767;
    }

    //Joystick
    if (deadzoneOffset > 0 && abs(axisEvent.value) < deadzoneOffset) {
        // return center value
        return 63;
    }
    int maxVal = (32767 - deadzoneOffset);
    if (axisEvent.value > 0) {
        return (axisEvent.value - deadzoneOffset + maxVal) * (127.0f / (2 * maxVal));
    }
    return (axisEvent.value + deadzoneOffset + maxVal) * (127.0f / (2 * maxVal));
}

juce::String MainComponent::decodeAxis(int axis) {
    switch (axis) {
    case 0:
        return juce::String("LS X Val: ");
    case 1:
        return juce::String("LS Y Val: ");
    case 2:
        return juce::String("RS X Val: ");
    case 3:
        return juce::String("RS Y Val: ");
    case 4:
        return juce::String("L2 Val: ");
    case 5:
        return juce::String("R2 Val: ");
    default:
        // I don't anticipate hitting this error,. but if I do, this handling is weak.
        return juce::String("<undefined axis>: " + juce::String(axis));
    }
}

void MainComponent::sendToConsole() {
    std::scoped_lock lock(midiLogMutex);
    for (const auto& msg : midiLogQueue) {
        midiConsole->appendMessage(msg);
    }

    midiLogQueue.clear();
    return;
}

void MainComponent::SDLPolling() {
    SDL_Event event;
    SDL_Gamepad* myController;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED: {
            myController = SDL_OpenGamepad(event.gdevice.which);
            // DBG("New Controller Connected Named: " << SDL_GetGamepadName(myController));
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            myController = SDL_OpenGamepad(event.gdevice.which);
            // DBG("Controller Removed Named: " << SDL_GetGamepadName(myController));
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
            // DBG("Controller Button Down: ");
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_UP: {
            // DBG("Controller Button Up: ");
            break;
        }
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            // Joystick Deadzone
            int axisVal = axisConversion(event.gaxis);
            if (axisVal == 63 && event.gaxis.axis < 4) {
                break;
            }

            // Log Event
            DBG(decodeAxis(event.gaxis.axis) << axisVal);

            // Send a MIDI Message to my virtual port
            midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(1, event.gaxis.axis + sceneOffset, axisVal));
            break;
        }
        case SDL_EVENT_QUIT: {
            DBG("SDL_QUIT received.");
            break;
        }
        default: {
            break;
        }
        }
    }
    return;
}

void MainComponent::myOscFunction() {
    juce::OSCSender oscSender;

    bool connected = oscSender.connect("127.0.0.1", 9000);
    if (!connected) {
        DBG("OSC Connection Failed;");
    }
    juce::OSCMessage msg("/transport/play", 1);
    oscSender.send(msg);
    return;
}