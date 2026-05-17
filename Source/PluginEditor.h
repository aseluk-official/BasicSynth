/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor&);
    ~BasicSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BasicSynthAudioProcessor& audioProcessor;
    
    juce::MidiKeyboardComponent keyboardComponent;
    juce::ToggleButton sinButton, triangleButton, sawButton;
    juce::Label sinLabel, triangleLabel, sawLabel;
    juce::TextEditor globalTranspose, sinTranspose, triangleTranspose, sawWaveTranspose, sinTransposeLabel, triangleTransposeLabel, sawTransposeLabel, globalTransposeLabel;
    juce::Slider sinTransposeSlider, triangleTransposeSlider, sawTransposeSlider, globalTransposeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSynthAudioProcessorEditor)
};
