#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class CtControlLayer : public CustomPaths {
	
public:
	
    CtControlLayer(string name);
    ~CtControlLayer(){}
	
	void setup();
	void update();
    void draw(int path);
    void idle();
    void resume();

private:
    float max_size;
    float max_pulse_size_factor;

};

