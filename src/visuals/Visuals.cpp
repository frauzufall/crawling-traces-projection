#include "Visuals.h"

using namespace guardacaso;

Visuals::Visuals() {

    content_w.set("content width", 0);
    content_h.set("content height", 0);

}

ofParameter<float> &Visuals::contentWidth() {
    return content_w;
}

ofParameter<float> &Visuals::contentHeight() {
    return content_h;
}

ofPoint Visuals::inContent(ofPoint orig) {
    ofPoint res = orig;
    res.x*=contentWidth();
    res.y*=contentHeight();
    return res;
}

Visuals::~Visuals() {
}
