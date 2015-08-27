#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class EmptyPaths : public CustomPaths {
	
public:
	
    EmptyPaths(string name);
    ~EmptyPaths(){}
	
    void setup();
    void update(ofx2DMappingProjector *projector, map<string, DrawingObject_ptr> &clients);
    void draw(ofx2DMappingProjector *projector, map<string, DrawingObject_ptr> &clients);
    void idle();
    void resume();
	
};

