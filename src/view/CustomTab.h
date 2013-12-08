#pragma once
#include "ofxGui.h"

namespace guardacaso {

    class CustomTab : public ofxPanel
    {
    public:
        CustomTab():ofxPanel() {

            visible = true;
            TTF.loadFont("fonts/Ubuntu-L.ttf", 10);

        }

        void draw(ofPoint p) {

            if(p != relative_pos) {
                relative_pos = p;
                this->setPosition(relative_pos+placing.getPosition());
            }

            if(visible) {
                ofxPanel::draw();
            }

        }

        ofTrueTypeFont TTF;
        ofParameter<bool> visible;

        bool isVisible() {
            return visible;
        }

        void setVisible(bool visible) {
            this->visible = visible;
        }

        void setPlacing(ofRectangle rect) {
            placing = rect;
            this->setPosition(relative_pos+placing.getPosition());
        }

        ofRectangle getPlacing() {
            return placing;
        }

    private:
        ofRectangle placing;
        ofPoint relative_pos;

    };

}
