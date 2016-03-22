#include "PathsTab.h"
#include "Visuals.h"
#include "PathsController.h"
#include "MappingController.h"
#include "Stuff.h"

using namespace guardacaso;

PathsTab::PathsTab(const string &name, const ofJson &config):CustomTab(name, config){}

void PathsTab::setup(PathsController* pc){

	paths_controller = pc;

	setName("Paths");
	setConfig(group_config);

	add(save.set("SAVE"), toggle_config);
	save.addListener(paths_controller, &PathsController::savePaths);
	paths_list = addGroup("Style", group_config);
	paths_options = addGroup("Style options", group_config);

	paths_list->setExclusiveToggles(true);

	paths_list->getActiveToggleIndex().addListener(paths_controller, &PathsController::activePathChanged);
	paths_list->getActiveToggleIndex().addListener(this, &PathsTab::setGui);

	int bla = 0;
	setGui(bla);

	for(uint i = 0; i < paths_controller->getPathsNames().size(); i++) {
		paths_list->add(paths_controller->getPath(i)->isActive().set(
				paths_controller->getPath(i)->getName(),
				paths_controller->getPath(i)->isActive()), toggle_config);
	}

	paths_options->setAttribute("float", LayoutFloat::LEFT);

}

PathsTab::~PathsTab() {
	paths_list->getActiveToggleIndex().removeListener(paths_controller, &PathsController::activePathChanged);
	paths_list->getActiveToggleIndex().removeListener(this, &PathsTab::setGui);
	save.removeListener(paths_controller, &PathsController::savePaths);
}

void PathsTab::setGui(int& index) {
	CustomPaths_ptr cp = paths_controller->getActivePath();
	paths_options->setShowHeader(cp->getSettings().size() > 0);
	paths_options->clear();
	paths_options->add(cp->getSettings());
	setNeedsRedraw();
}

