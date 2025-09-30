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

    // This group of members are the different sections of the application window. 
    juce::StretchableLayoutManager layoutManager;
    std::unique_ptr<MainContent> mainScreen;
    std::unique_ptr<MidiConsoleComponent> midiConsole;
    std::unique_ptr<ComponentResizerBar> resizerBar;
    std::unique_ptr<ControllerSelector> controllerSelector; // Currently not doing anything, but I wanted to add the ability to select acvite controller(s) at some point

    // MIDI Console Related
    bool isConsoleVisible = false;  
    void toggleConsole();
    void sendToConsole();
    std::mutex midiLogMutex;
    juce::StringArray midiLogQueue;

    // For MIDI I/O
    std::unique_ptr<juce::MidiInput> midiInput;
    std::unique_ptr<juce::MidiOutput> midiOutput;

    // SDL Related
    void SDLPolling();
    juce::String decodeAxis(int axis);
    float axisConversion(int axisVal, int axis);

    // Controller Related
    SDL_Gamepad* controller;
    void showControllerSelector();
    int deadzoneOffset;
    int sceneOffset; // Currently unused I believe, This was to be used for swapping mapped controller contexts

    void setSceneOffset(int offset);
    std::unordered_map<SDL_GamepadAxis, bool> isResting;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
