/*
  ==============================================================================

    ControllerSelector.h
    Created: 1 Jul 2025 10:32:12pm
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <SDL3/SDL.h>

/*
    This component can rescan for connected controllers. It knows the list of controller JoystickID's from the last time that SDL was initialized/scanned for controllers
*/

class ControllerSelector : public juce::Component {
public:
    ControllerSelector();
    ~ControllerSelector() override = default;
    void paint(juce::Graphics& g) override;
    void resized() override;
    SDL_Gamepad* getController();
    int initializeSDL();
    int scanControllers();
    void changeController();

private:
    SDL_JoystickID* scannedGamepadIDs;
    int numGamepads;
    int selectedGamepad;

    juce::TextButton rescanButton{ "Rescan Controllers" };
    juce::ComboBox controllerDropdown;
    juce::Label statusLabel;

};

