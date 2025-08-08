/*
  ==============================================================================

    ControllerSelector.cpp
    Created: 1 Jul 2025 10:32:12pm
    Author:  aerdn

  ==============================================================================
*/

#include "ControllerSelector.h"

ControllerSelector::ControllerSelector() {
    int status = initializeSDL();
    selectedGamepad = 0;
    controllerDropdown.setSelectedId(0);

    addAndMakeVisible(rescanButton);
    addAndMakeVisible(controllerDropdown);
    addAndMakeVisible(statusLabel);

    rescanButton.onClick = [this] { scanControllers(); };
    controllerDropdown.onChange = [this] { changeController(); };
}
void ControllerSelector::paint(juce::Graphics & g) {

}
void ControllerSelector::resized() {

}
SDL_Gamepad* ControllerSelector::getController() {
    return SDL_OpenGamepad(scannedGamepadIDs[0]);
}
int ControllerSelector::initializeSDL() {
    if (SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_EVENTS) < 0) {
        DBG("SDL Failed");
        Uint32 initted = SDL_WasInit(0);
        if (initted & SDL_INIT_GAMEPAD)
            DBG("GAMEPAD subsystem initialized");
        else
            DBG("GAMEPAD subsystem NOT initialized");

        if (initted & SDL_INIT_EVENTS)
            DBG("EVENTS subsystem initialized");
    }
    else {
        DBG("SDL Initialized Successfully!");

        // Get the default controller
        scanControllers();

        // Check out what we got
        if (scannedGamepadIDs == NULL) {
            DBG("SDL_GetGamepads() Failed");
        }
        else {
            DBG("SDL_GetGamepads() Succeeded");

            juce::String message = "numGamePads: " + juce::String(numGamepads);
            DBG(message);

            if (numGamepads > 0) {
                SDL_Gamepad* myController = SDL_OpenGamepad(scannedGamepadIDs[0]);
                juce::String controllerName = SDL_GetGamepadName(myController);
                message = "Controller Name: " + controllerName;
                DBG(message);
            }
            else {
                DBG("No Controllers Detected At Startup");
            }

            DBG("\n\n\n");
        }
    }
    // Success!
    return 0;
}

int ControllerSelector::scanControllers() {

    controllerDropdown.clear();
    scannedGamepadIDs = nullptr;
    scannedGamepadIDs = SDL_GetGamepads(&numGamepads);

    for (int i = 0; i < numGamepads; i++) {
        SDL_Gamepad* myController = SDL_OpenGamepad(scannedGamepadIDs[i]);
        controllerDropdown.addItem(SDL_GetGamepadName(myController), i);
    }

    statusLabel.setText("Select a Controller", juce::dontSendNotification);

    return numGamepads;
}

void ControllerSelector::changeController() {
    selectedGamepad = controllerDropdown.getSelectedId();
    // At this point, we need to communicate to the main component that the controller is changing.
    return;
}
