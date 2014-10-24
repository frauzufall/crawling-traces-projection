#pragma once

#include "CustomTab.h"

namespace guardacaso {

class ServerTab : public CustomTab {

    public:
        ServerTab();

        void update();
        void draw(ofPoint p);

    private:

        void drawServerStatus(ofRectangle shape);

        ofxButton save_btn;
        CustomTab participants;

        ofRectangle control_rect;
        ofParameter<string> timeout_idle;
        ofParameter<string> timeout_gone;

};

}
