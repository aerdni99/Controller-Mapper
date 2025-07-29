#pragma once

#include <JuceHeader.h>

class PS4ControllerGraphic : public juce::Component
{
public:
	PS4ControllerGraphic();
	~PS4ControllerGraphic();

	void paint(juce::Graphics&) override;
	void resized() override;

	void updateButtonState(juce::String buttonID, bool isPressed);
	void updateJoystickPosition(juce::String stickID, float x, float y);
};
