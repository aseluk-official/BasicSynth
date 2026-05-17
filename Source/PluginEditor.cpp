/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicSynthAudioProcessorEditor::BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent);
    setWantsKeyboardFocus(true);
    
    addAndMakeVisible(sinButton);
    addAndMakeVisible(triangleButton);
    addAndMakeVisible(sawButton);
    
    addAndMakeVisible(sinLabel);
    addAndMakeVisible(triangleLabel);
    addAndMakeVisible(sawLabel);
    
    addAndMakeVisible(sinTransposeLabel);
    addAndMakeVisible(triangleTransposeLabel);
    addAndMakeVisible(sawTransposeLabel);
    addAndMakeVisible(globalTransposeLabel);
    
    sinLabel.attachToComponent(&sinButton, true);
    triangleLabel.attachToComponent(&triangleButton, true);
    sawLabel.attachToComponent(&sawButton, true);
    
    sinLabel.setText("Sine wave", juce::NotificationType::dontSendNotification);
    triangleLabel.setText("Triangle wave", juce::NotificationType::dontSendNotification);
    sawLabel.setText("Saw wave", juce::NotificationType::dontSendNotification);

    sinTransposeLabel.setText("Sine Transpose", juce::NotificationType::dontSendNotification);
    triangleTransposeLabel.setText("Triangle Transpose", juce::NotificationType::dontSendNotification);
    sawTransposeLabel.setText("Saw Transpose", juce::NotificationType::dontSendNotification);
    globalTransposeLabel.setText("Global Transpose", juce::NotificationType::dontSendNotification);
    
    sinButton.onClick = [this] {
        audioProcessor.sinWaveOn.store(sinButton.getToggleState());
    };
    triangleButton.onClick = [this] {
        audioProcessor.triangleWaveOn.store(triangleButton.getToggleState());
    };
    sawButton.onClick = [this] {
        audioProcessor.sawWaveOn.store(sawButton.getToggleState());
    };
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);
}

BasicSynthAudioProcessorEditor::~BasicSynthAudioProcessorEditor()
{
}

//==============================================================================
void BasicSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Mysterious Synth by aseluk\nAka: Ahmet Selim Uslu", getLocalBounds(), juce::Justification::centred, 1);
}

void BasicSynthAudioProcessorEditor::resized()
{
    const auto labelWidth = 20;
    const auto labelHeight = 10;
    const auto buttonWidth = 25;
    const auto buttonHeight = 20;
    
    sinLabel.setBounds(labelWidth, labelHeight * 0, labelWidth, labelHeight);
    triangleLabel.setBounds(labelWidth, labelHeight * 1, labelWidth, labelHeight);
    sawLabel.setBounds(labelWidth, labelHeight * 2, labelWidth, labelHeight);
    sinButton.setBounds(80, buttonHeight * 0, buttonWidth, buttonHeight);
    triangleButton.setBounds(80, buttonHeight * 1, buttonWidth, buttonHeight);
    sawButton.setBounds(80, buttonHeight * 2, buttonWidth, buttonHeight);
    
    keyboardComponent.setBounds (10, getHeight() - 110, getWidth() - 20, 100);
}
