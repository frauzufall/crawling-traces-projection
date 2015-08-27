#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class CtControlLayer : public CustomPaths {
	
public:
	
    CtControlLayer(string name);
    ~CtControlLayer(){}
	
	void setup();
    void update(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients);
    void draw(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients);
    void idle();
    void resume();

private:

};

