#include "PathsTab.h"
#include "Visuals.h"
#include "PathsController.h"
#include "MappingController.h"
#include "Stuff.h"

using namespace guardacaso;

PathsTab::PathsTab():CustomTab(){}

void PathsTab::setup(PathsController* pc){

    paths_controller = pc;

    ofxGuiGroup::setup("Paths", group_config);

    paths_list.setup("Style", group_config);
    paths_list.setExclusiveToggles(true);

    ofAddListener(paths_list.activeToggleChanged, paths_controller, &PathsController::activePathChanged);
    ofAddListener(paths_list.activeToggleChanged, this, &PathsTab::setGui);

    paths_options.setup("Style options", group_config);
    int bla = 0;
    setGui(bla);

    paths_names = paths_controller->getPathsNames();
    for(uint i = 0; i < paths_names.size(); i++) {
        paths_list.add(paths_controller->getPath(i)->isActive().set(
                paths_controller->getPath(i)->getName(),
                paths_controller->getPath(i)->isActive()), toggle_config);
    }

    save_btn.setup("SAVE", toggle_config);
    save_btn.setPosition(margin,0);
    add(save_btn);
    save_btn.addListener(paths_controller, &PathsController::savePaths);
    add(paths_list);
    add(paths_options);

    paths_list.setPosition(save_btn.getPosition()+ofPoint(0,50));
    paths_options.setPosition(paths_list.getShape().getTopRight()+ofPoint(20, 0));

}

PathsTab::~PathsTab() {
    ofRemoveListener(paths_list.activeToggleChanged, paths_controller, &PathsController::activePathChanged);
    ofRemoveListener(paths_list.activeToggleChanged, this, &PathsTab::setGui);
    save_btn.removeListener(paths_controller, &PathsController::savePaths);
}

void PathsTab::setGui(int& index) {
    CustomPaths_ptr cp = paths_controller->getActivePath();
    paths_options.setShowHeader(cp->getSettings().size() > 0);
    paths_options.clear();
    ofxGuiGroup::Config c = group_config;
    c.showHeader = false;
    paths_options.add(cp->getSettings(), c);
    setNeedsRedraw();
}

