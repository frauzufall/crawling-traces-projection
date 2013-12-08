#include "CustomPaths.h"
#include "Visuals.h"
#include "PathsController.h"

using namespace guardacaso;
	
CustomPaths::CustomPaths(string title) {

    setup_done = false;
    loaded = false;

    active.set(false);

//    active.addListener(&PathsController::getInstance(),&PathsController::activePathChanged);

	delay = 1000/70;
    lastupdate = 0;
//    std::transform(title.begin(), title.end(),title.begin(), ::toupper);
    name = title;

    name_sl1 = "";
    name_sl2 = "";
    name_sl3 = "";
    name_rsl1 = "";
    name_rsl2 = "";
    name_rsl3 = "";
    name_btn1_1 = "";
    name_btn1_2 = "";
    name_btn2_1 = "";
    name_btn2_2 = "";
    name_btn3_1 = "";
    name_btn3_2 = "";

}

void CustomPaths::updatePaths() {

    if(!setup_done) {
        setup_done = true;
        setup();
    }

    loaded = true;

    data.outlines = Visuals::get().outlines();

}

string CustomPaths::getName() {
	return name;
}

bool CustomPaths::isSetupDone() {
    return setup_done;
}

bool CustomPaths::isLoaded() {
    return loaded;
}

PathsData* CustomPaths::getData() {
    return &data;
}

void CustomPaths::setData(PathsData * d) {
    data.copyFrom(d);
}

ofParameter<bool> CustomPaths::isActive() {
    return active;
}

void CustomPaths::setActive(bool active) {
    this->active.set(active);
}
