#include "MySynthVoice.h"

MySynthVoice::MySynthVoice() {
    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.25f;
    adsr.setParameters(adsrParams);
}

void MySynthVoice::updateParameters(bool sinWaveOn, bool triangleWaveOn, bool sawWaveOn){
    this->sinWaveOn.store(sinWaveOn);
    this->triangleWaveOn.store(triangleWaveOn);
    this->sawWaveOn.store(sawWaveOn);
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
    double p = phase - floor(phase);
    return (float)(2.0 * p - 1.0);
}

void MySynthVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!adsr.isActive()) {
        clearCurrentNote();
        return;
    }
    
    const bool doSin = sinWaveOn.load();
    const bool doTri = triangleWaveOn.load();
    const bool doSaw = sawWaveOn.load();
    const int numActive = (int)doSin + (int)doTri + (int)doSaw;
    if (level <= 0 || numActive == 0) return;
    
    const double sinePhaseDelta     = phaseDelta * globalTranspose * sineTranspose;
    const double trianglePhaseDelta = phaseDelta * globalTranspose * triangleTranspose;
    const double sawPhaseDelta      = phaseDelta * globalTranspose * sawTranspose;
    
    while (--numSamples >= 0){
        float adsrValue = adsr.getNextSample();
        
        auto currentSample = 0.0f;
        
        if (doSin) {
            currentSample += (float) std::sin (sinePhase) * level * 0.2f;
            updatePhase(sinePhase, sinePhaseDelta);
        } 
        if (doTri) {
            currentSample += triangleWave(trianglePhase) * level * 0.2f;
            updatePhase(trianglePhase, trianglePhaseDelta);
        }
        if (doSaw) {
            currentSample += sawWave(sawPhase) * level * 0.2f;
            updatePhase(sawPhase, sawPhaseDelta);
        }
        
        currentSample /= numActive;
        currentSample *= adsrValue;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
            outputBuffer.addSample(channel, startSample, currentSample);
        }
        currentPhase += phaseDelta;
        if (currentPhase >= juce::MathConstants<double>::twoPi)
            currentPhase -= juce::MathConstants<double>::twoPi;
        ++startSample;
    }
};
void MySynthVoice::updatePhase(double& phase, double delta) {
    phase += delta;
    if (phase >= juce::MathConstants<double>::twoPi)
        phase -= juce::MathConstants<double>::twoPi;
}
