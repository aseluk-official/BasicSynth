#include "MySynthVoice.h"

MySynthVoice::MySynthVoice(){}
bool MySynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<MySynthSound*> (sound) != nullptr;
};
void MySynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
            
    auto sampleRate = getSampleRate();
    if (sampleRate > 0.0)
        phaseDelta = (cyclesPerSecond * juce::MathConstants<double>::twoPi) / sampleRate;

    currentPhase = 0.0;
    level = velocity;
};
void MySynthVoice::stopNote (float velocity, bool allowTailOff) {
    clearCurrentNote();
    level = 0.0;
}
void MySynthVoice::pitchWheelMoved (int newPitchWheelValue) {};
void MySynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {};
void MySynthVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (level <= 0) return;
    
    while (--numSamples >= 0){
        auto currentSample = (float) std::sin (currentPhase) * level * 0.2f;
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
            outputBuffer.addSample(channel, startSample, currentSample);
        }
        currentPhase += phaseDelta;
        ++startSample;
    }
};
