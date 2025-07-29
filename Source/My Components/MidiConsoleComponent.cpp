/*
  ==============================================================================

    MidiConsoleComponent.cpp
    Created: 28 Jun 2025 4:50:06pm
    Author:  aerdn

  ==============================================================================
*/

#include "MidiConsoleComponent.h"

MidiConsoleComponent::MidiConsoleComponent() {
    startTimerHz(30);
}

void MidiConsoleComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::green);
    g.setFont(juce::Font("Consolas", 14.0f, juce::Font::plain));

    auto area = getLocalBounds().reduced(4);
    int lineHeight = 16;

    for (int i = std::max(0, displayLines.size() - area.getHeight() / lineHeight); i < displayLines.size(); ++i) {
        g.drawText(displayLines[i], area.removeFromTop(lineHeight), juce::Justification::left);
    }
}

void MidiConsoleComponent::resized() {
    // No Child Components
}

void MidiConsoleComponent::appendMessage(const juce::String& msg) {
    pendingLines.add(msg);
}

void MidiConsoleComponent::clear() {
    return;
}

void MidiConsoleComponent::timerCallback() {
    if (pendingLines.isEmpty()) return;

    displayLines.addArray(pendingLines);
    pendingLines.clear();

    while (displayLines.size() > maxLines) {
        displayLines.remove(0);
    }

    repaint();
}
