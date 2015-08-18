#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class EmptyPaths : public CustomPaths {
	
public:
	
    EmptyPaths(string name);
    ~EmptyPaths(){}
	
    void setup();
    void update(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients);
    void draw(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients);
    void idle();
    void resume();
	
};

