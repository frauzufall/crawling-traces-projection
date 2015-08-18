#include "app.h"

using namespace guardacaso;

#define USETRACES true

//AppStart::AppStart() {
//    arguments.resize(0);
//}

//--------------------------------------------------------------
void AppStart::setup(){

    ofSetLogLevel(OF_LOG_FATAL_ERROR);

//    ofSetFrameRate(20);

    ofBackground(0);

    fullscreen = false;

//    SoundController::getInstance().setup();
    mapping_controller.setup();

    if(arguments.size() == 3) {
        traces_controller.setup(&mapping_controller, arguments.at(1), ofToInt(arguments.at(2)));
    }
    else {
        traces_controller.setup(&mapping_controller);
    }

    paths_controller.setup();

    int w = mapping_controller.getMapping()->getControl()->controlWidth()
            +mapping_controller.getMapping()->getControl()->getProjector()->outputWidth();
    int h = max(
                mapping_controller.getMapping()->getControl()->controlHeight().get(),
                mapping_controller.getMapping()->getControl()->getProjector()->outputHeight());
    ofSetWindowShape(w,h);

    control_window.setup(&mapping_controller,
                         &paths_controller,
                         &traces_controller);

    delay = 1000/24;
    lastupdate = 0;

}

//--------------------------------------------------------------
void AppStart::update(){

    int currenttime = ofGetElapsedTimeMillis();
    if(currenttime-lastupdate > delay) {

        lastupdate = currenttime;

        traces_controller.update();
        paths_controller.update(mapping_controller.getMapping()->getControl()->getProjector()->outlines(),
                                traces_controller.getObjectController()->getClients());
        mapping_controller.update();
//        SoundController::getInstance().update();

        control_window.update();
    }

}

//--------------------------------------------------------------
void AppStart::draw(){

    mapping_controller.draw();
    ofPushMatrix();
    if(mapping_controller.controlLeft()){
        ofTranslate(mapping_controller.getMapping()->getControl()->getProjector()->outputWidth(),0);
    }
    paths_controller.draw(mapping_controller.getMapping()->getControl()->getProjector()->outlines(),
                          traces_controller.getObjectController()->getClients());
    ofPopMatrix();
    control_window.draw();

}

void AppStart::exit(){
}

//--------------------------------------------------------------
void AppStart::keyPressed(int key){

    switch(key) {
    case 's':
        mapping_controller.controlLeft().set(!mapping_controller.controlLeft());
        break;
    case 'f':
        ofToggleFullscreen();
        break;
//    case 'i':
//        Traces::get().simulateGroups();
//        ControlWindow::getInstance().getMapping().reload();
//        break;
    default:break;
    }

}

//--------------------------------------------------------------
void AppStart::keyReleased(int key){
}

//--------------------------------------------------------------
void AppStart::mouseMoved(ofMouseEventArgs &args){

}

//--------------------------------------------------------------
void AppStart::mouseDragged(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void AppStart::mousePressed(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void AppStart::mouseReleased(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void AppStart::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void AppStart::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void AppStart::dragEvent(ofDragInfo dragInfo){

}

//void AppStart::audioOut(float * input, int bufferSize, int nChannels) {
//    SoundController::getInstance().audioOut(input, bufferSize, nChannels);
//}
