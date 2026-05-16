#pragma once
#include <JuceHeader.h>
#include "MySynthSound.h"

class MySynthVoice : public juce::SynthesiserVoice{
    public:
        MySynthVoice();
        bool canPlaySound (juce::SynthesiserSound* sound) override;
        void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
        void stopNote (float velocity, bool allowTailOff) override;
        void pitchWheelMoved (int newPitchWheelValue) override;
        void controllerMoved (int controllerNumber, int newControllerValue) override;
        void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
        // Wave Functions
        float triangleWave(double phase);
        float sawWave(double phase);
    
        uint8_t numberOfWaves = 2;
        float lowpassFilterCutoff;
    private:
        double currentPhase = 0.0;
        double phaseDelta = 0.0;
        float level = 0.0;
        juce::ADSR adsr;
        juce::ADSR::Parameters adsrParams;
};
