#pragma once
#include <JuceHeader.h>

class MySynthSound : public juce::SynthesiserSound{
    public:
    MySynthSound();
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
};
