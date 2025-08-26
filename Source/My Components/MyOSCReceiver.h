/*
  ==============================================================================

    MyOSCReceiver.h
    Created: 25 Aug 2025 2:02:09am
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*   reference https://docs.juce.com/master/classOSCReceiver.html
*   reference https://www.youtube.com/watch?v=klv1O_1wuio
*/
class MyOSCReceiver  : public juce::Component,
    public juce::OSCReceiver,
    public juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
public:
    MyOSCReceiver();
    ~MyOSCReceiver() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void oscMessageReceived(const juce::OSCMessage& message) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyOSCReceiver)
};
