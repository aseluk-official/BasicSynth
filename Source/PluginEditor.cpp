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
    
    addAndMakeVisible(sinTransposeSlider);
    addAndMakeVisible(triangleTransposeSlider);
    addAndMakeVisible(sawTransposeSlider);
    addAndMakeVisible(globalTransposeSlider);
    
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
    
    sinTransposeSlider.setRange(-24, 24, 1);
    triangleTransposeSlider.setRange(-24, 24, 1);
    sawTransposeSlider.setRange(-24, 24, 1);
    globalTransposeSlider.setRange(-24, 24, 1);
    
    sinTransposeSlider.onValueChange = [this] {
        audioProcessor.sinTranspose.store(sinTransposeSlider.getValue());
    };
    triangleTransposeSlider.onValueChange = [this] {
        audioProcessor.sinTranspose.store(triangleTransposeSlider.getValue());
    };
    sawTransposeSlider.onValueChange = [this] {
        audioProcessor.sinTranspose.store(sawTransposeSlider.getValue());
    };
    globalTransposeSlider.onValueChange = [this] {
        audioProcessor.sinTranspose.store(globalTransposeSlider.getValue());
    };
    
    sinButton.onClick = [this] {
        audioProcessor.sinWaveOn.store(sinButton.getToggleState());
    };
    triangleButton.onClick = [this] {
        audioProcessor.triangleWaveOn.store(triangleButton.getToggleState());
    };
    sawButton.onClick = [this] {
        audioProcessor.sawWaveOn.store(sawButton.getToggleState());
    };
    
    addAndMakeVisible (resetButton);
    
    resetButton.setButtonText("Reset");
    resetButton.onClick = [this] {
        resetValues();
    };

    resetValues();
    
    setSize (800, 300);
}

void BasicSynthAudioProcessorEditor::resetValues(){
    sinTransposeSlider.setValue(-12);
    triangleTransposeSlider.setValue(0);
    sawTransposeSlider.setValue(0);
    globalTransposeSlider.setValue(-24);
    
    sinButton.setToggleState(true, juce::sendNotification);
    triangleButton.setToggleState(true, juce::sendNotification);
    sawButton.setToggleState(false, juce::sendNotification);
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
    g.setFont (juce::FontOptions(20.0f));
    
    auto topArea = getLocalBounds().removeFromTop(220);
    
    g.drawFittedText ("Mysterious Synth by aseluk\nAKA: Ahmet Selim Uslu", topArea, juce::Justification::centred, 1);
}

void BasicSynthAudioProcessorEditor::resized()
{
    const auto labelWidth = 20;
    const auto labelHeight = 20;
    const auto buttonWidth = 25;
    const auto buttonHeight = 20;
    
    sinLabel.setBounds(labelWidth, labelHeight * 0, labelWidth, labelHeight);
    triangleLabel.setBounds(labelWidth, labelHeight * 1, labelWidth, labelHeight);
    sawLabel.setBounds(labelWidth, labelHeight * 2, labelWidth, labelHeight);
    sinButton.setBounds(80, buttonHeight * 0, buttonWidth, buttonHeight);
    triangleButton.setBounds(80, buttonHeight * 1, buttonWidth, buttonHeight);
    sawButton.setBounds(80, buttonHeight * 2, buttonWidth, buttonHeight);
    
    const auto sliderWidth = 49 * 5;
    const auto sliderHeight = 20;
    
    const auto bigLabelWidth = labelWidth * 6;
    
    sinTransposeLabel.setBounds(getWidth() - (sliderWidth + bigLabelWidth), labelHeight * 0, bigLabelWidth, labelHeight);
    triangleTransposeLabel.setBounds(getWidth() - (sliderWidth + bigLabelWidth), labelHeight * 1, bigLabelWidth, labelHeight);
    sawTransposeLabel.setBounds(getWidth() - (sliderWidth + bigLabelWidth), labelHeight * 2, bigLabelWidth, labelHeight);
    globalTransposeLabel.setBounds(getWidth() - (sliderWidth + bigLabelWidth), labelHeight * 3, bigLabelWidth, labelHeight);
    
    sinTransposeSlider.setBounds(getWidth() - sliderWidth, sliderHeight * 0, sliderWidth, sliderHeight);
    triangleTransposeSlider.setBounds(getWidth() - sliderWidth, sliderHeight * 1, sliderWidth, sliderHeight);
    sawTransposeSlider.setBounds(getWidth() - sliderWidth, sliderHeight * 2, sliderWidth, sliderHeight);
    globalTransposeSlider.setBounds(getWidth() - sliderWidth, sliderHeight * 3, sliderWidth, sliderHeight);
    
    
    int keyboardX = 10;
    int keyboardHeight = 100;
    int keyboardWidth = getWidth() - 20;
    int keyboardY = getHeight() - 110;

    keyboardComponent.setBounds (keyboardX, keyboardY, keyboardWidth, keyboardHeight);

    int resetButtonWidth = 80;
    int resetButtonHeight = 30;
    int resetButtonMargin = 5;

    resetButton.setBounds (getWidth() / 2 - resetButtonWidth / 2,
                           keyboardY - resetButtonHeight - resetButtonMargin,
                           resetButtonWidth,
                           resetButtonHeight);
}
