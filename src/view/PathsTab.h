#pragma once

#include "CustomTab.h"

namespace guardacaso {

class PathsTab : public CustomTab {

    public:
        PathsTab();

        void update();
        void draw(ofPoint pos);
        void setGui();

    private:

        vector<string> paths_names;

        bool is_setup;

        ofRectangle control_rect;

        float preview_h, preview_w;

        string active_path;

        ofxButton save_btn;

        CustomTab paths_list, paths_options;

};

}
