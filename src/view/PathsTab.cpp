#include "PathsTab.h"
#include "Visuals.h"
#include "PathsController.h"
#include "MappingController.h"
#include "Stuff.h"

using namespace guardacaso;

PathsTab::PathsTab():CustomTab()
{
    setup("PATHS");

    setBorderColor(ofColor::black);
    setHeaderBackgroundColor(ofColor(255,0,61));

    paths_list.setup("LIST");
    paths_list.setBorderColor(ofColor::black);
    paths_list.setHeaderBackgroundColor(ofColor(159,0,38));
    paths_options.setup("OPTIONS");
    paths_options.setBorderColor(ofColor::black);
    paths_options.setHeaderBackgroundColor(ofColor(159,0,38));

    paths_names = PathsController::getInstance().getPathsNames();
    for(uint i = 0; i < paths_names.size(); i++) {
        paths_list.add(PathsController::getInstance().getPath(i)->isActive().set(
                PathsController::getInstance().getPath(i)->getName(),
                PathsController::getInstance().getPath(i)->isActive()));
    }


    save_btn.addListener(&Visuals::get(), &Visuals::savePaths);
    save_btn.setup("SAVE");
    add(&save_btn);

    active_path = -1;

    ofxPanel::registerMouseEvents();

}

void PathsTab::setGui() {

    CustomPaths_ptr cp = PathsController::getInstance().getActivePath();

    active_path = cp->getName();

    paths_options.clear();

    if(cp->name_btn1_1 != "") {
        paths_options.add(cp->getData()->btn1_1.set(
            cp->name_btn1_1, cp->getData()->btn1_1));
    }
    if(cp->name_btn1_2 != "") {
        paths_options.add(cp->getData()->btn1_2.set(
            cp->name_btn1_2, cp->getData()->btn1_2));
    }
    if(cp->name_rsl1 != "") {
        paths_options.add(cp->getData()->rsl1.set(
            cp->name_rsl1, cp->getData()->rsl1, 0, 1));
    }
    if(cp->name_sl1 != "") {
        paths_options.add(cp->getData()->sl1.set(
            cp->name_sl1, cp->getData()->sl1, 0, 1));
    }

    if(cp->name_btn2_1 != "") {
        paths_options.add(cp->getData()->btn2_1.set(
            cp->name_btn2_1, cp->getData()->btn2_1));
    }
    if(cp->name_btn2_2 != "") {
        paths_options.add(cp->getData()->btn2_2.set(
            cp->name_btn2_2, cp->getData()->btn2_2));
    }
    if(cp->name_rsl2 != "") {
        paths_options.add(cp->getData()->rsl2.set(
            cp->name_rsl2, cp->getData()->rsl2, 0, 1));
    }
    if(cp->name_sl2 != "") {
        paths_options.add(cp->getData()->sl2.set(
            cp->name_sl2, cp->getData()->sl2, 0, 1));
    }

    if(cp->name_btn3_1 != "") {
        paths_options.add(cp->getData()->btn3_1.set(
            cp->name_btn3_1, cp->getData()->btn3_1));
    }
    if(cp->name_btn3_2 != "") {
        paths_options.add(cp->getData()->btn3_2.set(
            cp->name_btn3_2, cp->getData()->btn3_2));
    }
    if(cp->name_rsl3 != "") {
        paths_options.add(cp->getData()->rsl3.set(
            cp->name_rsl3, cp->getData()->rsl3, 0, 1));
    }
    if(cp->name_sl3 != "") {
    paths_options.add(cp->getData()->sl3.set(
            cp->name_sl3, cp->getData()->sl3, 0, 1));
    }

    paths_options.add(cp->getData()->color.set(
            "color", cp->getData()->color.get(),
            ofColor(0,0),ofColor(255,255)));
}

void PathsTab::update() {

    if(control_rect.position != this->getPosition()) {
        control_rect.position = this->getPosition();
        control_rect.setWidth(this->getWidth());
        control_rect.setHeight(this->getHeight());

        paths_list.setPlacing(ofRectangle(this->getPosition().x, this->getPosition().y+this->getHeight()+10,0,0));
        paths_options.setPlacing(ofRectangle(this->getPosition().x+this->getWidth()+10, this->getPosition().y,0,0));
    }

    if(active_path != PathsController::getInstance().getActivePath()->getName()) {
        setGui();
    }
}

void PathsTab::draw(ofPoint pos) {

    if(visible) {

        CustomTab::draw(pos);
        paths_list.draw(pos);
        paths_options.draw(pos);

    }

}
