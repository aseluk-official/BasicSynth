#include "MySynthSound.h"

MySynthSound::MySynthSound(){};
bool MySynthSound::appliesToNote (int midiNoteNumber) {
    return true;
};
bool MySynthSound::appliesToChannel (int midiChannel) {
    return true;
};
