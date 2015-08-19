#include "MappingController.h"
#include "Visuals.h"
#include "PathsController.h"

using namespace guardacaso;

MappingController::MappingController() {
    control_left.set("control left", true);
}

MappingController::~MappingController() {
    control_left.removeListener(this, &MappingController::updatePosition);
}

void MappingController::setup(){

    xml_mapping = "sessions/last/mapping.xml";

    mapping.addTemplate<ofx2DMappingPoint>("start");
    mapping.addTemplate<ofx2DMappingShape>("drawing area");
    mapping.addColorTemplate("window", ofColor(0));

    mapping.setup(xml_mapping, false);
    mapping.showControls(false);

    bool left = control_left;
    updatePosition(left);
    control_left.addListener(this, &MappingController::updatePosition);

}

void MappingController::update() {

    mapping.update();

}

void MappingController::draw(int projector_id) {

    ofx2DMappingProjector* p = mapping.getControl()->getProjector(projector_id);

    ofSetColor(255);

    mapping.draw();

    //draw white borders around windows for calibration
    if(mapping.getControl()->getCalibrating()){
        mapping.getControl()->drawCalibration(p);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    ofEnableAlphaBlending();

}

ofx2DMapping* MappingController::getMapping(){
    return &mapping;
}

void MappingController::updatePosition(bool &left){
    if(left){
        mapping.getControl()->setOutputPosition(mapping.getControl()->getProjector()->outputWidth(),0);
    }else {
        mapping.getControl()->setOutputPosition(0,0);
    }
}

void MappingController::importSvg(const std::string path) {

    mapping.getControl()->getProjector()->importSvg(path);
    update();

}


ofParameter<bool> &MappingController::controlLeft() {
    return control_left;
}

ofPoint MappingController::controlPoint() {
    if(controlLeft())
        return ofPoint(0,0);
    else
        return ofPoint(mapping.getControl()->getProjector()->outputWidth(),0);
}
