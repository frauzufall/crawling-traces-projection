#pragma once

#include "ofxSvg.h"
#include "DrawingObject.h"

typedef std::shared_ptr<ofxSVG> ofxSVG_ptr;
typedef std::shared_ptr< vector<ofPolyline> > ofPolylines_ptr;
typedef std::shared_ptr<DrawingObject> DrawingObject_ptr;

class PathsData {

private:

public:

    void update();

    float                       step;

    PathsData();

    void copyFrom(PathsData *d);

    ~PathsData();

};
