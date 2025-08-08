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

class MainContent : public juce::Component
{
public:
    MainContent();
    ~MainContent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    std::function<void()> onShowControllerSelector;
    std::function<void()> onToggleConsole;

private:
    juce::TextButton menuButton{ "Menu" };
    juce::Label controllerLabel;
    bool isConsoleVisible = false;
};
