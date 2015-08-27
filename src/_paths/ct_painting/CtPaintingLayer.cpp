#include "CtPaintingLayer.h"

CtPaintingLayer::CtPaintingLayer(string name):CustomPaths(name) {
    settings.add(stroke1_w.set("stroke1 w", 1, 0.5, 20));
    settings.add(stroke2_w.set("stroke2 w", 1, 0.5, 20));
    settings.add(control_size.set("control size", 6, 1, 33));
    settings.add(alpha1.set("alpha1", 1, 0, 1));
    settings.add(alpha2.set("alpha2", 1, 0, 1));
    settings.add(pulse_size.set("pulse size", 1, 1, 4));
    settings.add(show_lines.set("show lines", true));
    settings.add(show_controls.set("show controls", true));
}

void CtPaintingLayer::setup() {
}

void CtPaintingLayer::update(ofx2DMappingProjector *projector, map<string, DrawingObject_ptr> &clients) {

}

void CtPaintingLayer::draw(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients) {

    ofEnableAlphaBlending();
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,
           GL_NICEST);
    ofNoFill();

    map<string,DrawingObject_ptr>::iterator iter;
    DrawingObject_ptr c;
    for( iter = clients.begin(); iter != clients.end(); iter++) {

        c = iter->second;

        if(c) {

            if(show_lines) {
                ofColor col = c->getColor();
                col.setBrightness(col.getBrightness()*1.1);

                ofSetLineWidth(stroke1_w);
                ofSetColor(col, col.a*alpha1);

                c->getConnections().draw();

                ofSetLineWidth(stroke2_w);
                ofSetColor(col, col.a*alpha2);

                c->getLine().draw();
            }

            if(show_controls) {
                ofColor col = c->getColor();
                col.setBrightness(col.getBrightness()*1.2);
                ofSetColor(col);
                ofNoFill();
                ofSetLineWidth(3);

                float current_time = ofGetElapsedTimef();
                float pulse_time = (int)(c->getPulseDuration()+1);
                float pulsing = (current_time-c->getPulseStart())/pulse_time;
                c->getPulseVal().set(pulsing);
                if(pulsing < 1) {
                    float tmp = fmodf(pulsing,1./pulse_time)*pulse_time;
                    float pulse_val = 1-(cos(tmp*2*PI)+1)/2;
                    ofDrawCircle(c->getPos(),control_size*(pulse_size*pulse_val+1));
                }
                else {
                    ofDrawCircle(c->getPos(),control_size);
                }
            }

        }

    }

}

void CtPaintingLayer::idle() {

}

void CtPaintingLayer::resume() {

}
