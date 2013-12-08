#pragma once

#include "ofxSvg.h"
#include "DrawingObject.h"

typedef tr1::shared_ptr<ofxSVG> ofxSVG_ptr;
typedef tr1::shared_ptr< vector<ofPolyline> > ofPolylines_ptr;
typedef tr1::shared_ptr<DrawingObject> DrawingObject_ptr;

class PathsData {

private:

public:

    ofParameter<ofColor> color;
    ofParameter<float> sl1,sl2,sl3;
    ofParameter<float> rsl1,rsl2,rsl3;
    ofParameter<bool> btn1_1,btn1_2,btn2_1,btn2_2,btn3_1,btn3_2;

    float                       step;
    ofPolylines_ptr             outlines, outlines_raw;
    ofxSVG_ptr                  svg;

    PathsData();

    void copyFrom(PathsData *d);

    map<string,DrawingObject_ptr> getDrawingObjects();

    float getPulseTime();

    ~PathsData();

};
