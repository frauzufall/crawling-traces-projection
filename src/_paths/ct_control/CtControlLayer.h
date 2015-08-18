#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class CtControlLayer : public CustomPaths {
	
public:
	
    CtControlLayer(string name);
    ~CtControlLayer(){}
	
	void setup();
    void update(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients);
    void draw(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients);
    void idle();
    void resume();

private:

};

