#include "ControlWindow.h"
#include "MappingController.h"
#include "ServerController.h"
#include "ObjectController.h"
#include "PathsController.h"
#include "Stuff.h"

#include "Traces.h"

#include <regex>
#include <iostream>
#include <string>

using namespace guardacaso;

ControlWindow::ControlWindow():ofxPanel() {
    is_setup = false;
}

bool ControlWindow::isSetup() {
    return is_setup;
}

void ControlWindow::setup(MappingController* mc, PathsController* pc, Traces* tc) {

    mapping_controller = mc;
    paths_controller = pc;
    traces_controller = tc;

    ofxPanel::setup("crawling traces");
    setShowHeader(false);

    xml_gui = "sessions/last/gui.xml";

    w = mapping_controller->getMapping()->getControl()->controlWidth();
    h = mapping_controller->getMapping()->getControl()->controlHeight();
    setSize(w+1,h);

    //system_temp_freq = 30000;
    //last_temp_update = -system_temp_freq;
    //status_temp = "";

    bool left = mapping_controller->controlLeft().get();
    updatePosition(left);
    mapping_controller->controlLeft().addListener(this, &ControlWindow::updatePosition);

    header.setup("header");
    header.setLayout(ofxBaseGui::Horizontal);
    header.setShowHeader(false);

    title.setup("title","crawling traces");
    title.setShowName(false);
    header.add(title);
    header.add<ofxFpsPlotter>();
    header.setBorderColor(ofColor(47));
    header.setDefaultBackgroundColor(ofColor(47));
    //status.setup("STATUS");

    save_settings_btn.addListener(this, &ControlWindow::saveAllSettings);
    header.add(save_settings_btn.setup("Save all settings"));

//    import_events_btn.addListener(this, &ControlWindow::importGroup);
//    import_events_btn.setup("Import events");
//    gui.add(&import_events_btn);

    //status.add(status_temp.set("Hardware temperatures", ".. showing status .."));

    server.setup(traces_controller);
    paths.setup(paths_controller);

    gui.setup("tabbed page");

    gui.setShowHeader(false);
    gui.setTabHeight(50);
    gui.setTabWidth(100);
    gui.setSize(w,h-header.getHeight()-10);
    gui.add(server);
    gui.add(paths);
    gui.add(*mapping_controller->getMapping()->getControlView());
    //gui.setSize(w,h-header.getHeight()-10);

    add(header);
    addSpacer(10);
    add(gui);

    is_setup = true;

}

void ControlWindow::update() {

    paths.update();

//    float current_time = ofGetElapsedTimeMillis();
//    if(current_time - last_temp_update >= system_temp_freq) {

//        last_temp_update = current_time;

//        // get NVIDIA GPU and CPU temperature

//        stringstream temp_str, tmp_rm;

//        string status_gpu = Stuff::exec("sensors | grep temp7");

//        tmp_rm << "temp 7:";
//        status_gpu.erase(status_gpu.begin(), status_gpu.begin()+tmp_rm.str().size());
//        Stuff::trim(status_gpu);

//        temp_str << "\nGPU: " << status_gpu << "\n";

//        int cpu_count = atoi(Stuff::exec("sensors | grep Core | wc -l").c_str());

//        for(int i = 0; i < cpu_count; i++) {

//            stringstream command;
//            command << "sensors | grep Core | sed -n " << i+1 << "p";
//            string status_cpu = Stuff::exec(command.str().c_str());
//            Stuff::trim(status_cpu);

//            temp_str << status_cpu << "\n";

//        }

//        status_temp = temp_str.str();
//    }

}

ControlWindow::~ControlWindow() {
    mapping_controller->controlLeft().addListener(this, &ControlWindow::updatePosition);
    save_settings_btn.removeListener(this, &ControlWindow::saveAllSettings);
}

void ControlWindow::saveAllSettings() {
    //MappingController::getInstance().getMapping()->getControl()->saveMappingDefault();
    traces_controller->saveServer();
    paths_controller->savePaths();
}

void ControlWindow::importGroup() {
    traces_controller->simulateGroups(mapping_controller);
    //TODO update mapping tab
}

void ControlWindow::updatePosition(bool &left){
    if(left){
        this->setPosition(0,0);
    }else {
        this->setPosition(mapping_controller->getMapping()->getControl()->getProjector()->outputWidth(),0);
    }

}
