/*
  ==============================================================================

    mainContent.h
    Created: 1 Jul 2025 5:02:37pm
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <SDL3/SDL.h>
#include "MyOSCReceiver.h"
#include "AssignsTable.h"


class MainContent : public juce::Component
{
public:
    MainContent();
    ~MainContent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    std::function<void()> onShowControllerSelector;
    std::function<void()> onToggleConsole;

    void setSceneOffset(int offset);
    juce::String decodeAxis(int rowNum);
    void postParamVal(int axis, float val);

    void setSelRow(int rowNum);

private:
    juce::TextButton menuButton{ "Menu" };
    juce::Label controllerLabel;
    bool isConsoleVisible = false;
    AssignsTable assignTable;
    int sceneOffset;
    int selRow;

    // JUCE OSC related (Open Sound Protocol)
    void myOscSender(juce::String route, float value, juce::String path); // for sending OSC messages
    MyOSCReceiver OSCReceiver; // for receiving OSC messages

    juce::Array<juce::var> mappedParams;
    float adjustParamVal(float min, float max, float value);
};
