/*
  ==============================================================================

    mainContent.cpp
    Created: 1 Jul 2025 5:02:37pm
    Author:  aerdn

  ==============================================================================
*/

#include "mainContent.h"

MainContent::MainContent() {

    // Top Left Menu Button ====================================================
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

    // Mapping button event handling ===========================================
    assignTable.onButtonClicked = [this](const int route, const bool mapping) {
        rowSelection = route;
        if (!mapping) {
            if (mappedParams[rowSelection].isVoid()) {
                DBG("No Parameter Mapped to " << decodeAxis(rowSelection));
            }
            else {
                DBG("Clearing Parameter: " << mappedParams[rowSelection].getDynamicObject()->getProperty("name").toString() << " at " << decodeAxis(rowSelection));
                mappedParams.set(rowSelection, juce::var());
            }
        }
        else {
            myOscSender(route);
        }
        return;
    };
    addAndMakeVisible(assignTable);

    // OSC Receiver Message Handling ===========================================
    OSCReceiver.onMessageReceived = [this](const juce::OSCMessage& msg) {
        if (msg.getAddressPattern().toString() == "/is/playing") {
            int playState = msg[0].getInt32();
            DBG("Transport play state: " << playState);
        }
        else if (msg.getAddressPattern().toString() == "/mapped/parameter") {
            juce::var parsed = juce::JSON::parse(msg[0].getString());
            mappedParams.set(rowSelection, parsed);
            DBG("Paraemeter Mapped: " << mappedParams[rowSelection].getDynamicObject()->getProperty("name").toString());
        }
        else if (msg.getAddressPattern().toString() == "/clear") {


        }
        return;
    };

    // Set Parameter Mappings to NULL by Default ===============================
    mappedParams.resize(6);
}

void MainContent::paint(juce::Graphics& g) {
    menuButton.setBounds(10, 10, 60, 24);
    assignTable.setBounds(80, 44, 552, 162); // row height 22 * 6 rows + header height 28 + 2 border = 162
    g.fillAll(juce::Colours::white);
}
void MainContent::resized() {
}

void MainContent::myOscSender(int route) {
    /*
        route 0 - LSX
        route 1 - LSY
        route 2 - RSX
        route 3 - RSY
        route 4 - L2
        route 5 - R2
    */

    juce::OSCSender oscSender;
    bool connected = oscSender.connect("127.0.0.1", 9000);
    if (!connected) {
        DBG("OSC Connection Failed;");
    }

    juce::OSCMessage msg("/map", route + sceneOffset);
    oscSender.send(msg);
    return;
}

void MainContent::setSceneOffset(int offset) {
    sceneOffset = offset;
    return;
}

// This function is copied from MainComponent. I should find a way to eliminate the copy/paste later if I get around to it. For better practice's sake
juce::String MainContent::decodeAxis(int rowNum) {
    switch (rowNum) {
    case 0:
        return juce::String("LS X");
    case 1:
        return juce::String("LS Y");
    case 2:
        return juce::String("RS X");
    case 3:
        return juce::String("RS Y");
    case 4:
        return juce::String("L2");
    case 5:
        return juce::String("R2");
    default:
        // I don't anticipate hitting this error,. but if I do, this handling is weak.
        return juce::String("<undefined axis>: " + juce::String(rowNum));
    }
}