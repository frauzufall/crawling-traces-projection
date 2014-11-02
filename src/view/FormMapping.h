#pragma once

#include "ofMain.h"
#include "CustomTab.h"

namespace guardacaso {

struct draggableVertex {

    float 	x;
    float 	y;
    bool 	bBeingDragged;
    bool 	bOver;
    float 	radius;
    int     quadIndex;

    ofPoint asPoint() {
        return ofPoint(x,y);
    }

};


class FormMapping : public CustomTab {

public:
    FormMapping();

    bool mouseMoved(ofMouseEventArgs &args);
    bool mouseDragged(ofMouseEventArgs &args);
    bool mousePressed(ofMouseEventArgs &args);
    bool mouseReleased(ofMouseEventArgs &args);

    void updateForms(int projector_id);
    void update();
    void draw(ofPoint pos, int projector_id);

    void setMappingRects();

private:

    void updateQuadList();

    vector< vector < draggableVertex > >  quads_dst;
    draggableVertex start_point;
    vector <draggableVertex> camera;
    ofRectangle mapping_rect_dst, control_rect;

    ofColor img_col;
    ofColor pic_col;
    ofColor win_col;
    ofColor cam_col;

    ofColor getColorOf(string name);

    ofParameter<string> explanation;

    bool use_cam;

};

}
