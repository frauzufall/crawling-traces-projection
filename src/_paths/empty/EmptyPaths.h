#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class EmptyPaths : public CustomPaths {
	
public:
	
    EmptyPaths(string name);
    ~EmptyPaths(){}
	
    void setup();
    void update(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients);
    void draw(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients);
    void idle();
    void resume();
	
};

