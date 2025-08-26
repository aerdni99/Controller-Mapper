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
    connect(8000);
    addListener(this, "/juce");
    addListener(this, "/transport/play");
    addListener(this, "/view/zoom");
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
    if (message.getAddressPattern().toString() == "/transport/play") {
        int playState = message[0].getInt32();
        DBG("Transport play state: " << playState);
    }
    else if (message.getAddressPattern().toString() == "/view/zoom") {
        float zoomLevel = message[0].getFloat32();
        DBG("Zoom Level: " << zoomLevel);
    }
    else if (message.getAddressPattern().toString() == "/juce") {
        int randNum = message[0].getInt32();
        DBG("Random Number: " << randNum);
    }
}