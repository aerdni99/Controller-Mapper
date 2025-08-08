/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ControllerMapperAudioProcessorEditor::ControllerMapperAudioProcessorEditor (ControllerMapperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //midiVolume.addListener(this);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 800);
    setResizable(true, false);
    setResizeLimits(400, 300, 1000, 800);

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
}

ControllerMapperAudioProcessorEditor::~ControllerMapperAudioProcessorEditor()
{
}

//==============================================================================
void ControllerMapperAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().reduced(10);

    // (Our component is white
    g.fillAll(juce::Colours::lightgrey);
    g.setColour(juce::Colours::white);
    g.fillRect(area);
}

void ControllerMapperAudioProcessorEditor::resized()
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

void ControllerMapperAudioProcessorEditor::toggleConsole() {
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

void ControllerMapperAudioProcessorEditor::logMidiMessage(const juce::MidiMessage& message) {
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

    midiConsole->appendMessage(msgText);
}

void ControllerMapperAudioProcessorEditor::timerCallback() {
    std::scoped_lock lock(audioProcessor.midiLogMutex);

    for (const auto& msg : audioProcessor.midiLogQueue) {
        midiConsole->appendMessage(msg);
    }

    audioProcessor.midiLogQueue.clear();
}

void ControllerMapperAudioProcessorEditor::showControllerSelector() {

}