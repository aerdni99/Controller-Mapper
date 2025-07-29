/*
  ==============================================================================

    MidiConsoleComponent.h
    Created: 28 Jun 2025 4:50:06pm
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MidiConsoleComponent : public juce::Component,
    juce::Timer
{
public:
    MidiConsoleComponent();
	~MidiConsoleComponent() override = default;


    void paint(juce::Graphics& g) override;
    void resized() override;

    void appendMessage(const juce::String& msg);
    void clear();

    void timerCallback() override;

private:
    juce::StringArray pendingLines;
    juce::StringArray displayLines;

    static constexpr int maxLines = 200;
};

