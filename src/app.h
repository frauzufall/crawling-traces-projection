#pragma once

#include "ofMain.h"
//#include "ControlWindow.h"
//#include "MappingController.h"

//using namespace guardacaso;

class AppStart : public ofBaseApp {

public:
    AppStart() {}
    ~AppStart() {
    }
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void scrolled(float deltaX, float deltaY);

private:

    int delay;
    int lastupdate;

    bool first_round;

    bool fullscreen;

};
