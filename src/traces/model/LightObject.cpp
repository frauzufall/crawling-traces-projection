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

void LightObject::setId(string id) {
    _id = id;
}

void LightObject::setName(string n) {
    myname = n;
}

void LightObject::setType(string t) {
    mytype = t;
}

void LightObject::setColor(ofColor c) {
    color = c;
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

string LightObject::getId() {
    return _id;
}

string LightObject::getName() {
    return myname;
}

string LightObject::getType() {
    return mytype;
}

ofColor LightObject::getColor() {
    return color;
}

ofPoint LightObject::getPos() {
    return pos;
}

ofPoint LightObject::getLastPos() {
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

void LightObject::setFadingOut(bool fading) {
    fading_out = fading;
}

bool LightObject::getFadingOut() {
    return fading_out;
}

void LightObject::setFadingOutStart(float status) {
    fading_out_start = status;
}

float LightObject::getFadingOutStart() {
    return fading_out_start;
}

void LightObject::setGone(bool is_gone) {
    gone = is_gone;
}

bool LightObject::isGone() {
    return gone;
}
