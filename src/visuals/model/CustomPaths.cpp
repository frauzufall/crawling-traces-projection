#include "CustomPaths.h"
#include "MappingController.h"
#include "PathsController.h"

using namespace guardacaso;
	
CustomPaths::CustomPaths(string title) {

    setup_done.set(false);
    loaded = false;

    active.set(false);

//    active.addListener(&PathsController::getInstance(),&PathsController::activePathChanged);

    delay = 1000/70;
    lastupdate = 0;
//    std::transform(title.begin(), title.end(),title.begin(), ::toupper);
    name = title;

}

string CustomPaths::getName() {
    return name;
}

ofParameter<bool>& CustomPaths::setupDone() {
    return setup_done;
}

bool CustomPaths::isLoaded() {
    return loaded;
}

ofParameter<bool>& CustomPaths::isActive() {
    return active;
}

ofParameterGroup& CustomPaths::getSettings(){
    return settings;
}
