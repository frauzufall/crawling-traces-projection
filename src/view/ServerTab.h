#pragma once

#include "Traces.h"
#include "CustomTab.h"

namespace guardacaso {

class ServerTab : public CustomTab {

    public:
        ServerTab();
        ~ServerTab();

        void setup(Traces* tc);
        void render();

        void updateTimeoutGone(float&);
        void updateTimeoutIdle(float&);
        void updateServerStatus(bool&);

    private:

        Traces* traces_controller;

        std::string secondsToTimestring(int seconds);

        ofxButton save_btn;
        ofxPanel settings;
        ofRectangle participants;
        ofxLabel status_msg;

        ofTrueTypeFont TTF;

        ofParameter<string> timeout_idle;
        ofParameter<string> timeout_gone;

};

}
