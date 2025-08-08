#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
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

    // Start timer for polling midi messages
    startTimerHz(30);

    // Initialize GUI components
    resized();

    // Initialize Controller
    controllerSelector = std::make_unique<ControllerSelector>();
    controller = controllerSelector->getController();

    // Midi input (for testing)
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    if (!midiInputs.isEmpty()) {
        auto deviceInfo = midiInputs[0];
        midiInput = juce::MidiInput::openDevice(deviceInfo.identifier, this);
        if (midiInput)
            midiInput->start();
    }

    deadzoneOffset = 4000;
}

MainComponent::~MainComponent()
{
}

//==============================================================================

void MainComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().reduced(10);

    // (Our component is white
    g.fillAll(juce::Colours::lightgrey);
    g.setColour(juce::Colours::white);
    g.fillRect(area);
}

void MainComponent::resized()
{
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

// This function 
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
    SDL_Event event;
    SDL_Gamepad* myController;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED: {
            myController = SDL_OpenGamepad(event.gdevice.which);
            DBG("New Controller Connected Named: " << SDL_GetGamepadName(myController));
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            myController = SDL_OpenGamepad(event.gdevice.which);
            DBG("Controller Removed Named: " << SDL_GetGamepadName(myController));
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
            DBG("Controller Button Down: ");
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_UP: {
            DBG("Controller Button Up: ");
            break;
        }
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            int axisVal = axisConversion(event.gaxis.value);
            if (axisVal != 63) {
                DBG("Axis Val: " << axisVal);
            }
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

    // log midi messages to my console
    std::scoped_lock lock(midiLogMutex);
    for (const auto& msg : midiLogQueue) {
        midiConsole->appendMessage(msg);
    }

    midiLogQueue.clear();
    return;
}

void MainComponent::showControllerSelector() {
    return;
}

int MainComponent::axisConversion(int axisVal) {
    if (deadzoneOffset > 0 && abs(axisVal) < deadzoneOffset) {
        // return center value
        return 63;
    }

    int maxVal = (32767 - deadzoneOffset);
    if (axisVal > 0) {
        return (axisVal - deadzoneOffset + maxVal) * (64.0f / maxVal);
    }
    return (axisVal + deadzoneOffset + maxVal)* (64.0f / maxVal);
}
