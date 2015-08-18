#include "LightObject.h"
#include "Stuff.h"

LightObject::LightObject() {
    myname = "unknown";
    mytype="unknown";
    color = ofColor(255);
    last_action = ofGetElapsedTimef();
    last_pos = ofPoint(0,0);
    pos = ofPoint(0,0);
    _id = "";
    fading_out = false;
    fading_out_start = 0;
    gone = false;
}

float LightObject::getLastActionTime() {
    return last_action;
}

string LightObject::getRestTimeAsString() {
    return Stuff::secondsToString(getTimeIdle());
}

float LightObject::getTimeIdle() {
    return ofGetElapsedTimef()-getLastActionTime();
}

float LightObject::getTimeGone() {
    return ofGetElapsedTimef()-getFadingOutStart();
}

ofParameter<string>& LightObject::getId() {
    return _id;
}

ofParameter<string>& LightObject::getName() {
    return myname;
}

ofParameter<string>& LightObject::getType() {
    return mytype;
}

ofParameter<ofColor>& LightObject::getColor() {
    return color;
}

ofParameter<ofPoint>& LightObject::getPos() {
    return pos;
}

ofParameter<ofPoint> &LightObject::getLastPos() {
    return last_pos;
}

ofColor LightObject::hsvToRgb(float h, float s, float v) {
    double      hh, p, q, t, ff;
    long        i;
    ofColor         out;

    if(s <= 0.0) {       // < is bogus, just shuts up warnings
        if(isnan(h)) {   // h == NAN
            out.r = v;
            out.g = v;
            out.b = v;
            return out;
        }
        // error - should never happen
        out.r = 0.0;
        out.g = 0.0;
        out.b = 0.0;
        return out;
    }
    hh = h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = v;
        break;
    case 5:
    default:
        out.r = v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

ofParameter<bool> &LightObject::getFadingOut() {
    return fading_out;
}

ofParameter<float> &LightObject::getFadingOutStart() {
    return fading_out_start;
}

ofParameter<bool> &LightObject::getIsGone() {
    return gone;
}
