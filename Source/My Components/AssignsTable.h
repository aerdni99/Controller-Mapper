/*
  ==============================================================================

    AssignsTable.h
    Created: 13 Aug 2025 12:00:03pm
    Author:  aerdn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AssignsTable  : public juce::Component,
    private juce::TableListBoxModel
{
public:
    AssignsTable();
    ~AssignsTable() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    std::function<void(const int, const bool)> onButtonClicked;
    void clearMapping(int rowNumber);
    void assignMapping(int rowNum, juce::String paramName);
    void processOSC(const juce::OSCMessage& msg);
    

private:
    juce::TableListBox table;

    struct RowData {
        juce::String name;
        juce::String descriptor;
    };
    juce::Array<RowData> rows;

    enum ColumnIds {
        colName = 1,
        colDescriptor = 2,
        colButton1 = 3,
        colButton2 = 4
    };

    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;

    juce::Array<juce::String> parameterNames;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AssignsTable)
};

