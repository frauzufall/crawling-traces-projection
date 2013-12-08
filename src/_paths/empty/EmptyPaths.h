#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

class EmptyPaths : public CustomPaths {
	
public:
	
    EmptyPaths(string name);
	
    void setup();
    void update();
    void draw(int path);
    void idle();
    void resume();
	
};

