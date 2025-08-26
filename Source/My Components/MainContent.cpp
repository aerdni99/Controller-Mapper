/*
  ==============================================================================

    mainContent.cpp
    Created: 1 Jul 2025 5:02:37pm
    Author:  aerdn

  ==============================================================================
*/

#include "mainContent.h"

MainContent::MainContent() {
    menuButton.onClick = [this]() {
        juce::PopupMenu menu;
        menu.addItem("Controllers...", [this]() {
            if (onShowControllerSelector)
                onShowControllerSelector();
            });

        menu.addItem("Show Midi Console", true, isConsoleVisible, [this]() {
            if (onToggleConsole)
                isConsoleVisible = !isConsoleVisible;
                onToggleConsole();
            });
        menu.addItem("Send OSC", [this]() {
            myOscSender();
            });

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton));

        };
    addAndMakeVisible(menuButton);
}
void MainContent::paint(juce::Graphics& g) {
    menuButton.setBounds(10, 10, 60, 24);
    g.fillAll(juce::Colours::white);
}
void MainContent::resized() {
}

void MainContent::myOscSender() {
    juce::OSCSender oscSender;

    bool connected = oscSender.connect("127.0.0.1", 9000);
    if (!connected) {
        DBG("OSC Connection Failed;");
    }
    juce::OSCMessage msg("/transport/play", 1);
    oscSender.send(msg);
    return;
}
