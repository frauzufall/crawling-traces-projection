#pragma once

#include "ofx2DMappingShape.h"

class DrawingArea : public ofx2DMappingShape {

public:

    DrawingArea():ofx2DMappingShape() {
        this->nature = "DRAWING_AREA";
    }

    ~DrawingArea(){}

//    DrawingArea(const DrawingArea& obj) : ofx2DMappingShape(obj) {
//    }

    ofPtr<ofx2DMappingObject> clone() const {
        return ofPtr<ofx2DMappingObject>(new DrawingArea(*this));
    }


    void draw(float w, float h){}

    void drawArea(float w, float h) {

        ofSetColor(this->color);

        ofFill();

        ofBeginShape();
        for (uint j = 0; j < this->polyline.size(); j++){
            ofVertex(this->polyline[j].x*w, this->polyline[j].y*h);
        }

        ofEndShape(true);

    }

protected:


};
