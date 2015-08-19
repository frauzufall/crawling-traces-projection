#pragma once

#include "PathsController.h"
#include "CustomTab.h"

namespace guardacaso {

class PathsTab : public CustomTab {

    public:
        PathsTab();
        ~PathsTab();

        void setup(PathsController* pc);
        void setGui(int &index);

    private:

        PathsController* paths_controller;

        vector<string> paths_names;

        float preview_h, preview_w;

        ofxButton save_btn;

        ofxGuiGroup paths_list, paths_options;

};

}
