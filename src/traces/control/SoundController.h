#pragma once

#include "ofMain.h"

class SoundController {

    public:
        static SoundController& getInstance();

        void setup();
        void update();
        void triggerActivity();

        void audioOut(float * input, int bufferSize, int nChannels);

    protected:
        SoundController();
        ~SoundController() {
        }

    private:

        ofSoundStream soundStream;

        float 	pan;
        int		sampleRate;
        bool 	bNoise;
        float 	volume, volume_max;

        vector <float> lAudio;
        vector <float> rAudio;

        //------------------- for the simple sine wave synthesis
        float 	targetFrequency;
        float 	phase;
        float 	phaseAdder;
        float 	phaseAdderTarget;

        float blendin, blendout;
        int soundscale;
        int last_activity;
        vector<int> activities;

};

