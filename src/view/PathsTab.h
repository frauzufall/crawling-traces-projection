#pragma once

#include "PathsController.h"
#include "ofxGuiPage.h"

namespace guardacaso {

class PathsTab : public ofxGuiPage {

    public:
        PathsTab();
        ~PathsTab();

        void setup(PathsController* pc);
        void update();
        void setGui();

    private:

        PathsController* paths_controller;

        vector<string> paths_names;

        float preview_h, preview_w;

        string active_path;

        ofxButton save_btn;

        ofxPanel paths_list, paths_options;

};

}
