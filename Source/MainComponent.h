#pragma once

#include <JuceHeader.h>
#include <SDL3/SDL.h>

#include "My Components/MidiConsoleComponent.h"
#include "My Components/ComponentResizerBar.h"
#include "My Components/ControllerSelector.h"
#include "My Components/MainContent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component,
    public juce::Timer,
    public juce::MidiInputCallback
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::StretchableLayoutManager layoutManager;

    std::unique_ptr<MainContent> mainScreen;
    std::unique_ptr<MidiConsoleComponent> midiConsole;
    std::unique_ptr<ComponentResizerBar> resizerBar;
    std::unique_ptr<ControllerSelector> controllerSelector;

    bool isConsoleVisible = false;

    void toggleConsole();
    void showControllerSelector();

    std::mutex midiLogMutex;
    juce::StringArray midiLogQueue;

    SDL_Gamepad* controller;

    std::unique_ptr<juce::MidiInput> midiInput;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
