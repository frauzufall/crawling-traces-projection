#include "EmptyPaths.h"
#include "Visuals.h"
#include "MappingController.h"

EmptyPaths::EmptyPaths(string name):CustomPaths(name) {}

void EmptyPaths::setup() {
    name_sl1 = "";
    name_sl2 = "";
    name_sl3 = "";
    name_rsl1 = "";
    name_rsl2 = "";
    name_rsl3 = "";
    name_btn1_1 = "";
    name_btn1_2 = "";
    name_btn2_1 = "";
    name_btn2_2 = "";
    name_btn3_1 = "";
    name_btn3_2 = "";
}

void EmptyPaths::update() {
}

void EmptyPaths::draw(int path) {

    MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(path);

    ofFill();

    if(mq->nature == "painting") {
        ofSetColor(data.color);
    }
    else {
        ofSetColor(0);
    }

    ofPolyline & line = data.outlines->at(path);

    ofBeginShape();
    for (int j = 0; j < line.size(); j++){
        ofVertex(line[j]);
    }
    ofEndShape(true);

}

void EmptyPaths::idle() {

}

void EmptyPaths::resume() {

}
