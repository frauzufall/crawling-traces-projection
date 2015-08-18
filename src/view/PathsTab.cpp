#include "PathsTab.h"
#include "Visuals.h"
#include "PathsController.h"
#include "MappingController.h"
#include "Stuff.h"

using namespace guardacaso;

PathsTab::PathsTab():ofxGuiPage(){}

void PathsTab::setup(PathsController* pc){

    paths_controller = pc;

    ofxGuiPage::setup("Paths");

    setBorderColor(ofColor(255,0,61));

    paths_list.setup("LIST");
    paths_list.setBorderColor(ofColor::black);
    paths_list.setHeaderBackgroundColor(ofColor(159,0,38));
    paths_list.setExclusiveToggles(true);
    paths_options.setup("OPTIONS");
    paths_options.setBorderColor(ofColor::black);
    paths_options.setHeaderBackgroundColor(ofColor(159,0,38));

    paths_names = paths_controller->getPathsNames();
    for(uint i = 0; i < paths_names.size(); i++) {
        paths_controller->getPath(i)->isActive().addListener(paths_controller, &PathsController::activePathChanged);
        paths_list.add(paths_controller->getPath(i)->isActive().set(
                paths_controller->getPath(i)->getName(),
                paths_controller->getPath(i)->isActive()));
    }


    save_btn.addListener(paths_controller, &PathsController::savePaths);
    save_btn.setup("SAVE");
    add(save_btn);
    add(paths_list);
    add(paths_options);

    paths_list.setPosition(save_btn.getPosition()+ofPoint(0,50));
    paths_options.setPosition(paths_list.getShape().getTopRight()+ofPoint(20, 0));

    active_path = -1;

}

PathsTab::~PathsTab() {
    for(uint i = 0; i < paths_controller->getPathsNames().size(); i++) {
        paths_controller->getPath(i)->isActive().removeListener(paths_controller, &PathsController::activePathChanged);
    }
    save_btn.removeListener(paths_controller, &PathsController::savePaths);
}

void PathsTab::setGui() {

    CustomPaths_ptr cp = paths_controller->getActivePath();

    active_path = cp->getName();

    paths_options.clear();

    paths_options.add(cp->getSettings());
}

void PathsTab::update() {
    if(paths_controller->getActivePath()){
        if(active_path != paths_controller->getActivePath()->getName()) {
            setGui();
        }
    }
}
