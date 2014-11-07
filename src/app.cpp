#include "app.h"
#include "Visuals.h"
#include "Traces.h"
//#include "InputController.h"
#include "MappingController.h"
#include "ServerController.h"
#include "ObjectController.h"
#include "PathsController.h"
#include "ControlWindow.h"
#include "SoundController.h"

using namespace guardacaso;

#define USETRACES true

AppStart::AppStart() {
    arguments.resize(0);
}

//--------------------------------------------------------------
void AppStart::setup(){

    ofSetLogLevel(OF_LOG_FATAL_ERROR);

    ofSetFrameRate(20);

    ofBackground(0);

    fullscreen = false;

    ofPushMatrix();
    ofPushStyle();
    if(arguments.size() == 3) {
        Traces::get().setup(arguments.at(1), ofToInt(arguments.at(2)));
    }
    else {
        Traces::get().setup();
    }
    SoundController::getInstance().setup();
    Visuals::get().setupMapping();
    Visuals::get().setupPaths();
    Visuals::get().setupOther();
    ofPopStyle();
    ofPopMatrix();

    int w = Visuals::get().controlWidth()+Visuals::get().outputWidth();
    int h = max(Visuals::get().controlHeight(),Visuals::get().outputHeight());
    ofSetWindowShape(w,h);

    ControlWindow::getInstance().setup();

    delay = 1000/24;
    lastupdate = 0;

    first_round = true;

}

//--------------------------------------------------------------
void AppStart::update(){

//    cout << ofGetFrameRate() << endl;

    int currenttime = ofGetElapsedTimeMillis();
    if(currenttime-lastupdate > delay) {

        lastupdate = currenttime;

        Traces::get().update();
        Visuals::get().update();
        SoundController::getInstance().update();

        if(ControlWindow::getInstance().isSetup())
            ControlWindow::getInstance().update();
        if(first_round) {

            first_round = false;
        }

    }

}

//--------------------------------------------------------------
void AppStart::draw(){

    ofPushMatrix();

    ofSetColor(255);

    if(Visuals::get().controlLeft()) {
        ofTranslate(Visuals::get().controlWidth(),0);
    }

    MappingController::getInstance().draw(0);

    ofPopMatrix();

    if(Visuals::get().controlLeft()) {
        ControlWindow::getInstance().draw(ofPoint(0,0));
    }
    else {
        ControlWindow::getInstance().draw(ofPoint(Visuals::get().outputWidth(),0));
    }

}

//--------------------------------------------------------------
void AppStart::keyPressed(int key){

    switch(key) {
    case 's':
        Visuals::get().setControlLeft(!Visuals::get().controlLeft());
        break;
    case 'f':
        ofToggleFullscreen();
        break;
//    case 'i':
//        Traces::get().simulateGroups();
//        ControlWindow::getInstance().getMapping().reload();
//        break;
    case '1':
        ControlWindow::getInstance().keyPressed(key);
        break;
    case '2':
        ControlWindow::getInstance().keyPressed(key);
        break;
    case '3':
        ControlWindow::getInstance().keyPressed(key);
        break;
    default:break;
    }

}

//--------------------------------------------------------------
void AppStart::keyReleased(int key){

}

//--------------------------------------------------------------
void AppStart::mouseMoved(int x, int y ){
//    cout << "mousemoved" << endl;
}

//--------------------------------------------------------------
void AppStart::mouseDragged(int x, int y, int button){
//    cout << "mousedragged" << endl;
}

//--------------------------------------------------------------
void AppStart::mousePressed(int x, int y, int button){
//    cout << "mousepressed" << endl;
    //ControlWindow::getInstance().mousePressed(x);
}

//--------------------------------------------------------------
void AppStart::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void AppStart::windowResized(int w, int h){

//    cout << "ATTENTION!!! _________ " << w << ":" << h << endl;

}

void AppStart::audioOut(float * input, int bufferSize, int nChannels) {
    SoundController::getInstance().audioOut(input, bufferSize, nChannels);
}
