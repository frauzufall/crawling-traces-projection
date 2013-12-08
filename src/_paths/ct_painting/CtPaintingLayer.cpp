#include "CtPaintingLayer.h"

CtPaintingLayer::CtPaintingLayer(string name):CustomPaths(name) {

}

void CtPaintingLayer::setup() {
    name_sl1 = "stroke1 w";
    name_sl2 = "stroke2 w";
    name_sl3 = "control size";
    name_rsl1 = "alpha1";
    name_rsl2 = "alpha2";
    name_rsl3 = "pulse size";
    name_btn1_1 = "show lines";
    name_btn1_2 = "show controls";
    name_btn2_1 = "";
    name_btn2_2 = "";
    name_btn3_1 = "";
    name_btn3_2 = "";

    max_pulse_size_factor = 4;
    max_size = 33;
}

void CtPaintingLayer::update() {

}

void CtPaintingLayer::draw(int path) {

    ofEnableAlphaBlending();
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,
           GL_NICEST);
    ofNoFill();

    map<string,DrawingObject_ptr>cs = data.getDrawingObjects();
    map<string,DrawingObject_ptr>::iterator iter;
    DrawingObject_ptr c;
    for( iter = cs.begin(); iter != cs.end(); iter++) {

        c = iter->second;

        if(c) {

            if(data.btn1_1) {
                ofColor col = c->getColor();
                col.setBrightness(col.getBrightness()*1.1);

                ofSetLineWidth(20*data.sl1);
                ofSetColor(col, col.a*data.rsl1);

                c->getConnections().draw();

                ofSetLineWidth(20*data.sl2);
                ofSetColor(col, col.a*data.rsl2);

                c->getLine().draw();
            }

            if(data.btn1_2) {
                ofColor col = c->getColor();
                col.setBrightness(col.getBrightness()*1.2);
                ofSetColor(col);
                ofNoFill();
                ofSetLineWidth(3);

                float current_time = ofGetElapsedTimef();
                float size = data.sl3*max_size;
                float pulse_time = (int)(data.getPulseTime()+1);
                float pulse_size_factor = (max_pulse_size_factor-1)*(data.rsl3)+1;
                float pulsing = (current_time-c->getPulseStart())/pulse_time;
                c->setPulseVal(pulsing);
                if(pulsing < 1) {
                    float tmp = fmodf(pulsing,1./pulse_time)*pulse_time;
                    float pulse_val = 1-(cos(tmp*2*PI)+1)/2;
                    ofCircle(c->getPos(),size*(pulse_size_factor*pulse_val+1));
                }
                else {
                    ofCircle(c->getPos(),size);
                }
            }

        }

    }

}

void CtPaintingLayer::idle() {

}

void CtPaintingLayer::resume() {

}
