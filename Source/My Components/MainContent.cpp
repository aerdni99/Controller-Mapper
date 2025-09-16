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

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton));

        };
    addAndMakeVisible(menuButton);
    addAndMakeVisible(assignTable);

    assignTable.onButtonClicked = [this](const int route, const bool mapping) {
        myOscSender(route, mapping);
    };

    OSCReceiver.onMessageReceived = [this](const juce::OSCMessage& msg) {
        assignTable.processOSC(msg);
    };
}
void MainContent::paint(juce::Graphics& g) {
    menuButton.setBounds(10, 10, 60, 24);
    assignTable.setBounds(80, 44, 552, 162); // row height 22 * 6 rows + header height 28 + 2 border = 162
    g.fillAll(juce::Colours::white);
}
void MainContent::resized() {
}

void MainContent::myOscSender(int route, bool mapping) {

    /*
        route 1 - LSX
        route 2 - LSY
        route 3 - RSX
        route 4 - RSY
        route 5 - L2
        route 6 - R2
    */
    juce::OSCSender oscSender;

    bool connected = oscSender.connect("127.0.0.1", 9000);
    if (!connected) {
        DBG("OSC Connection Failed;");
    }
    juce::String myRoute;

    switch (route) {
    case 1:
        myRoute = "/LSX";
        break;
    case 2:
        myRoute = "/LSY";
        break;
    case 3:
        myRoute = "/RSX";
        break;
    case 4:
        myRoute = "/RSY";
        break;
    case 5:
        myRoute = "/L2";
        break;
    case 6:
        myRoute = "/R2";
        break;
    default:
        myRoute = "/error";
        DBG("Bad osc message sent");
        break;
    }

    juce::OSCMessage msg(myRoute, mapping);
    oscSender.send(msg);
    return;
}
