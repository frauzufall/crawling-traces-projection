#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class CtControlLayer : public CustomPaths {
	
public:
	
    CtControlLayer(string name);
    ~CtControlLayer(){}
	
	void setup();
    void update(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients);
    void draw(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients);
    void idle();
    void resume();

private:

};

