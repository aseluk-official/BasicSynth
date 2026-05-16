#include "MySynthVoice.h"

MySynthVoice::MySynthVoice() {
    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.25f;
    adsr.setParameters(adsrParams);
}
bool MySynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<MySynthSound*> (sound) != nullptr;
};
void MySynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
            
    auto sampleRate = getSampleRate();
    if (sampleRate > 0.0)
        phaseDelta = (cyclesPerSecond * juce::MathConstants<double>::twoPi) / sampleRate;

    currentPhase = 0.0;
    
    adsr.setSampleRate(getSampleRate());
    adsr.noteOn();
    level = velocity;
};
void MySynthVoice::stopNote (float velocity, bool allowTailOff) {
    if (allowTailOff) {
        adsr.noteOff();
    }
    else {
        clearCurrentNote();
        adsr.reset();
    }
}
void MySynthVoice::pitchWheelMoved (int newPitchWheelValue) {};
void MySynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {};

float MySynthVoice::triangleWave(double phase){
    phase += 1;
    phase = fmod(phase, 2.0);
    phase -= 1.0;
    float result = (float) phase < 0 ? phase : -phase;
    return result;
}

float MySynthVoice::sawWave(double phase){
    phase = fmod(phase, 2.0);
    phase -= 1.0;
    float result = phase;
    return result;
}

void MySynthVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!adsr.isActive()) {
        clearCurrentNote();
        return;
    }
    if (level <= 0) return;
    
    const double globalTranspose = 0.3;
    
    const double sineTranspose = 1.0;
    const double triangleTranspose = 1.0;
    const double sawTranspose = 1.0 / 2.0;
    
    while (--numSamples >= 0){
        float adsrValue = adsr.getNextSample();
        
        auto currentSample = (float) std::sin (currentPhase * sineTranspose * globalTranspose) * level * 0.2f;
        currentSample += triangleWave(currentPhase * triangleTranspose * globalTranspose) * level * 0.2f;
        //currentSample += sawWave(currentPhase * sawTranspose * globalTranspose) * level * 0.2f;
        currentSample /= numberOfWaves;
        currentSample *= adsrValue;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
            outputBuffer.addSample(channel, startSample, currentSample);
        }
        currentPhase += phaseDelta;
        ++startSample;
    }
};
