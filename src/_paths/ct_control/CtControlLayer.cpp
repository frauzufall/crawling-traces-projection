#include "CtControlLayer.h"

CtControlLayer::CtControlLayer(string name):CustomPaths(name) {
	
}

void CtControlLayer::setup() {
    name_sl1 = "size";
    name_sl2 = "pulse s";
    name_sl3 = "";
    name_rsl1 = "";
    name_rsl2 = "";
    name_rsl3 = "";
    name_btn1_1 = "visible";
    name_btn1_2 = "";
    name_btn2_1 = "";
    name_btn2_2 = "";
    name_btn3_1 = "";
    name_btn3_2 = "";

    max_pulse_size_factor = 4;
    max_size = 33;
}

void CtControlLayer::update() {
}

void CtControlLayer::draw(int path) {

    if(data.btn1_1) {

        map<string,DrawingObject_ptr>cs = data.getDrawingObjects();
        map<string,DrawingObject_ptr>::iterator iter;
        DrawingObject_ptr c;
        for( iter = cs.begin(); iter != cs.end(); iter++ ) {

            c = iter->second;

            if(c) {

                ofColor col = c->getColor();
                col.setBrightness(col.getBrightness()*1.2);
                ofSetColor(col,255);
                ofNoFill();
                ofSetLineWidth(3);

                float current_time = ofGetElapsedTimef();
                float size = data.sl1*max_size;
                float pulse_time = (int)(data.getPulseTime()+1);
                float pulse_size_factor = (max_pulse_size_factor-1)*(data.sl2)+1;
                float pulsing = (current_time-c->getPulseStart())/pulse_time;
                c->setPulseVal(pulsing);
                if(pulsing < 1) {
                    float tmp = fmodf(pulsing,1./pulse_time)*pulse_time;
                    float pulse_val = 1-(cos(tmp*2*PI)+1)/2;
                    ofCircle(c->getPos(),size*(pulse_size_factor*pulse_val+1));
                }
                else {
                    ofCircle(c->getPos(),data.sl1*max_size);
                }
            }
        }
    }
}

void CtControlLayer::idle() {

}

void CtControlLayer::resume() {

}
