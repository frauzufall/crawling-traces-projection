#pragma once

#include "CustomPaths.h"
//#include "ofxBox2d.h"

using namespace guardacaso;

class CtPaintingLayer : public CustomPaths {
	
public:
	
    CtPaintingLayer(string name);
    ~CtPaintingLayer(){}
	
    void setup();
    void update();
    void draw(int path);
    void idle();
    void resume();

private:
    float max_size;
    float max_pulse_size_factor;
	
};
