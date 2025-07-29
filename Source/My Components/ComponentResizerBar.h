/*
  ==============================================================================

    ComponentResizerBar.h
    Created: 29 Jun 2025 4:50:06pm
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ComponentResizerBar : public juce::StretchableLayoutResizerBar {
public:
    ComponentResizerBar(juce::StretchableLayoutManager* layout, int index, bool isBarVertical)
        : StretchableLayoutResizerBar(layout, index, isBarVertical) {}

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::lightgrey);
    }
};