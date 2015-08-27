#pragma once

#include "CustomPaths.h"
#include "ofxBlur.h"

using namespace guardacaso;

struct BlurShape{
    ofxBlur blur;
    float width;
    float height;
};

class BricksLayer : public CustomPaths {
	
public:
	
    BricksLayer(string name);
    ~BricksLayer(){}
	
    void setup();
    void setBlurShapes(ofx2DMappingProjector *projector);
    void update(ofx2DMappingProjector *projector, map<string, DrawingObject_ptr> &clients);
    void draw(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients);
    void idle();
    void resume();

private:

    ofParameter<bool> blur_shapes_set;
    ofParameter<float> blur_scale;
    ofParameter<int> blur_radius;
    ofParameter<float> blur_shape;
    ofParameter<int> blur_passes;
    ofParameter<bool> blur_clear;
    ofParameter<int> brickrows;
    ofParameter<int> brickcols;
    ofParameter<float> brickoffset;

    ofParameter<bool> calibration;

    vector<ofPtr<BlurShape>> blurshapes;

    ofParameter<float> drawing_area_w;
    ofParameter<float> drawing_area_h;
    ofParameter<float> stroke1_w;
    ofParameter<float> stroke2_w;
    ofParameter<float> control_size;
    ofParameter<float> alpha1;
    ofParameter<float> alpha2;
    ofParameter<float> pulse_size;
    ofParameter<bool> show_lines;
    ofParameter<bool> show_controls;
	
};
