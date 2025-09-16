/*
  ==============================================================================

    AssignsTable.cpp
    Created: 13 Aug 2025 12:00:03pm
    Author:  aerdn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AssignsTable.h"

//==============================================================================
AssignsTable::AssignsTable()
{
    table.getHeader().addColumn("Name", colName, 150);
    table.getHeader().addColumn("Parameter", colDescriptor, 200);
    table.getHeader().addColumn("Map", colButton1, 100);
    table.getHeader().addColumn("Clear Mapping", colButton2, 100);

    table.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xffa1a1a1));
    table.setColour(juce::ListBox::outlineColourId, juce::Colour(0xff000000));
    table.setOutlineThickness(1);

    table.setModel(this);
    addAndMakeVisible(table);

    rows.add({ "LS-X", "N/A" });
    rows.add({ "LS-Y", "N/A" });
    rows.add({ "RS-X", "N/A" });
    rows.add({ "RS-Y", "N/A" });
    rows.add({ "L2", "N/A" });
    rows.add({ "R2", "N/A" });

    DBG("Header Height" << table.getHeader().getHeight());
}

AssignsTable::~AssignsTable()
{
    table.setModel(nullptr);
}

void AssignsTable::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xffa1a1a1));   // clear the background

    g.setColour (juce::Colour (0xff000000));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("AssignsTable", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AssignsTable::resized()
{
    table.setBounds(getLocalBounds());
}

int AssignsTable::getNumRows() {
    return rows.size();
}

void AssignsTable::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::lightblue);
    }
    else if (rowNumber % 2) {
        g.fillAll(juce::Colours::lightgrey);
    }
}

void AssignsTable::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    if (columnId == colName) {
        g.drawText(rows[rowNumber].name, 2, 0, width - 4, height, juce::Justification::centredLeft);
    }
    else if (columnId == colDescriptor) {
        g.drawText(rows[rowNumber].descriptor, 2, 0, width - 4, height, juce::Justification::centredLeft);
    }
}

juce::Component* AssignsTable::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (columnId == colButton1 || columnId == colButton2) {
        auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);

        if (btn == nullptr) {
            btn = new juce::TextButton();
        }
        if (columnId == colButton1) {
            btn->setButtonText("Map");
        }
        else {
            btn->setButtonText("Clear Mapping");
        }

        btn->onClick = [this, rowNumber, columnId] {
            // In here I need to initate talking to M4L. 

            if (columnId == colButton1) {
                //assignMapping();
                DBG("Mapping " << rows[rowNumber].name);
                if (onButtonClicked) {
                    onButtonClicked(rowNumber + 1, true);
                }
            }
            else {
                DBG("Clearing mapping for " << rows[rowNumber].name);
                if (onButtonClicked) {
                    onButtonClicked(rowNumber + 1, false);
                }
            }
        };
        return btn;
    }
    return nullptr;
}

void AssignsTable::clearMapping(int rowNumber) {

    return;
}

void AssignsTable::assignMapping(int rowNumber) {
    return;
}

void AssignsTable::processOSC(const juce::OSCMessage& msg) {
    // This function expects to see messages addressed like "/mapped"
    // The message should Have the parameter name and ID. The ID will be stored in secret so that we can clear it later, and the Name will be sent to the Parameter Column.


    // This logic is for playback state, not a concern of the AssignsTable, but it was the first OSC message processing that I did so I pasted it here for reference.
    /*if (msg.getAddressPattern().toString() == "/is/playing") {
        int playState = msg[0].getInt32();
        DBG("Transport play state: " << playState);
    }*/
    return;
}
