#pragma once

#include "ofMain.h"

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

private:

    int delay;
    int lastupdate;

    bool first_round;

    bool fullscreen;

};
