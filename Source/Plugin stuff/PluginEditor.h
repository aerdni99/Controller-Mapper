/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <SDL3/SDL.h>

#include "PluginProcessor.h"
#include "My Components/MidiConsoleComponent.h"
#include "My Components/ComponentResizerBar.h"
#include "My Components/MainContent.h"

//==============================================================================
/**
*/
class ControllerMapperAudioProcessorEditor : public juce::AudioProcessorEditor, 
    public juce::Timer
{
public:
    ControllerMapperAudioProcessorEditor (ControllerMapperAudioProcessor&);
    ~ControllerMapperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void logMidiMessage(const juce::MidiMessage& message);
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ControllerMapperAudioProcessor& audioProcessor;


    juce::StretchableLayoutManager layoutManager;

    std::unique_ptr<MainContent> mainScreen;
    std::unique_ptr<MidiConsoleComponent> midiConsole;
    std::unique_ptr<ComponentResizerBar> resizerBar;

    bool isConsoleVisible = false;

    void toggleConsole();
    void showControllerSelector();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControllerMapperAudioProcessorEditor)
};
