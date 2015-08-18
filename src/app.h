#pragma once

#include "ofMain.h"
#include "MappingController.h"
#include "Visuals.h"
#include "PathsController.h"
#include "ControlWindow.h"
#include "Traces.h"

using namespace guardacaso;

class AppStart : public ofBaseApp {

public:
//    AppStart();
//    ~AppStart() {
//    }
    void setup();
    void update();
    void draw();

    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(ofMouseEventArgs &args);
    void mouseDragged(ofMouseEventArgs &args);
    void mousePressed(ofMouseEventArgs &args);
    void mouseReleased(ofMouseEventArgs &args);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

//    void audioOut(float * input, int bufferSize, int nChannels);

    vector<string> arguments;

private:

    Visuals input_controller;
    MappingController mapping_controller;
    PathsController paths_controller;
    ControlWindow control_window;
    Traces traces_controller;

    int delay;
    int lastupdate;

    bool fullscreen;

};
