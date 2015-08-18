#pragma once

#include "ofMain.h"

class LightObject
{
    public:

        LightObject();

        ofParameter<string> &getId();
        ofParameter<string> &getName();
        ofParameter<string> &getType();
        int getHue();
        ofParameter<ofColor> &getColor();
        ofParameter<ofPoint> &getLastPos();
        ofParameter<ofPoint> &getPos();

        float getLastActionTime();
        float getTimeIdle();
        float getTimeGone();
        string getRestTimeAsString();
        ofParameter<bool>& getFadingOut();
        ofParameter<float>& getFadingOutStart();
        ofParameter<bool>& getIsGone();

        ~LightObject(){}

    protected:
        ofParameter<string> _id;
        ofParameter<string> myname;
        ofParameter<string> mytype;
        ofParameter<ofColor> color;
        ofParameter<ofPoint> pos;
        ofParameter<ofPoint> last_pos;
        ofParameter<float> last_action;
        ofParameter<bool> fading_out;
        ofParameter<float> fading_out_start;
        ofParameter<bool> gone;
        ofColor hsvToRgb(float h, float s, float v);
};
