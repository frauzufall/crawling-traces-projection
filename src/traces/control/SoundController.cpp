#include "SoundController.h"
#include "ServerController.h"
#include "Visuals.h"
#include "MappingController.h"

using namespace guardacaso;

SoundController &SoundController::getInstance() {
    static SoundController instance;
    return instance;
}

SoundController::SoundController() {
    int bufferSize		= 512;
    sampleRate 			= 44100;
    phase 				= 0;
    phaseAdder 			= 0.0f;
    phaseAdderTarget 	= 0.0f;
    volume				= 0;
    volume_max			= 0.1f;
    bNoise 				= false;

    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);

    blendin = 100;
    blendout = 2000;
    soundscale = 100;
    last_activity = -100000;
    activities.resize(0);

    soundStream.setup(ofGetAppPtr(), 2, 0, sampleRate, bufferSize, 4);
}

void SoundController::setup() {
}

void SoundController::triggerActivity() {
    int current_time = ofGetElapsedTimeMillis();
    activities.push_back(current_time);
    last_activity = current_time;
}

void SoundController::update() {

    int current_time = ofGetElapsedTimeMillis();

    //setting volume
    if(current_time-last_activity>blendout) {
        //muting
        volume = 0;
    }
    else {
        if(current_time-last_activity<blendin) {
            //blending in sound
            volume = volume_max*(float)(current_time-last_activity)/(float)blendin;
        }
        else {
            //blending out sound
            volume = volume_max*(1-(float)(current_time-last_activity)/(float)blendout);
        }
    }

    //setting height
    int cut = 0;
    for(int i = 0; i < activities.size(); i++) {
        if(activities.at(i) < current_time-blendout) {
            cut = i+1;
        }
    }
    //std::vector<decltype(activities)::int>(activities.begin()+cut, activities.end()).swap(activities);
    //std::vector<activities::int>(activities.begin()+cut, activities.end()).swap(activities);
    activities.erase(activities.begin(), activities.begin() + cut);
    float height = (float)soundscale;
    float heightPct = (activities.size() / height);
    targetFrequency = 2000.0f * heightPct;
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
}

void SoundController::audioOut(float * output, int bufferSize, int nChannels){
    //pan = 0.5f;
    float leftScale = 1 - pan;
    float rightScale = pan;

    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }

    if ( bNoise == true){
        // ---------------------- noise --------------
        for (int i = 0; i < bufferSize; i++){
            lAudio[i] = output[i*nChannels    ] = ofRandom(0, 1) * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = ofRandom(0, 1) * volume * rightScale;
        }
    } else {
        phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
        for (int i = 0; i < bufferSize; i++){
            phase += phaseAdder;
            float sample = sin(phase);
            lAudio[i] = output[i*nChannels    ] = sample * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
        }
    }

}
