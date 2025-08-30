/*
  ==============================================================================

    MyOSCReceiver.cpp
    Created: 25 Aug 2025 2:02:09am
    Author:  aerdn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MyOSCReceiver.h"

//==============================================================================
MyOSCReceiver::MyOSCReceiver() {
    if (!connect(8000)) {
        DBG("OSCReceiver failed to open port.");
    }
    addListener(this, "/is/playing");
}

MyOSCReceiver::~MyOSCReceiver() {
}

void MyOSCReceiver::paint (juce::Graphics& g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MyOSCReceiver", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void MyOSCReceiver::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void MyOSCReceiver::oscMessageReceived(const juce::OSCMessage& message) {
    if (onMessageReceived) {
        onMessageReceived(message);
    }
    return;
}