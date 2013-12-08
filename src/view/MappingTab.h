#pragma once

#include "CustomTab.h"
#include "SortableList.h";
#include "FormMapping.h"

namespace guardacaso {

class MappingTab : public CustomTab {

    public:
        MappingTab();

        void update();
        void draw(ofPoint pos);

        void reload();

        bool mouseMoved(ofMouseEventArgs& args);
        bool mouseDragged(ofMouseEventArgs &args);
        bool mousePressed(ofMouseEventArgs &args);
        bool mouseReleased(ofMouseEventArgs &args);

        void addCanvas();
        void addObstacle();
        void addImage();
        void removeForm(int index);
        void reorderForm(int old_index, int new_index);

    private:

        bool use_complex_images;

        void setGui();
        void updateQuadList();

        void importSvg();

        ofRectangle control_rect;

        ofxButton save_btn, import_svg_btn, add_obstacle_btn, add_canvas_btn, add_image_btn;
        SortableList mapping_list;
        FormMapping form_mapping;

        float mapping_space_rel;

        float zoom;
        ofPoint zoom_pos;

};

}
