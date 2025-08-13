/*
  ==============================================================================

    AssignsTable.h
    Created: 13 Aug 2025 12:00:03pm
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AssignsTable  : public juce::Component
{
public:
    AssignsTable();
    ~AssignsTable() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AssignsTable)
};
