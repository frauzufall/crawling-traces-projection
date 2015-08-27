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

void ControlWindow::setup(MappingController* mc, PathsController* pc, Traces* tc, VideoRecorderController* vrc) {

    mapping_controller = mc;
    paths_controller = pc;
    traces_controller = tc;
    video_recorder_controller = vrc;

    ofxPanel::setup("crawling traces");
    setShowHeader(false);
    setBorderColor(ofColor::fromHex(0x4BB3A5));

    xml_gui = "sessions/last/gui.xml";

    w = mapping_controller->getMapping()->getControl()->controlWidth();
    h = mapping_controller->getMapping()->getControl()->controlHeight();
    setSize(w+1,h);

    //system_temp_freq = 30000;
    //last_temp_update = -system_temp_freq;
    //status_temp = "";

    ofxBaseGui::Config config_header;
    config_header.backgroundColor = ofColor(0,0,0,0);
    config_header.shape.width = 170;
    config_header.shape.height = 30;

    bool left = mapping_controller->controlLeft().get();
    updatePosition(left);
    mapping_controller->controlLeft().addListener(this, &ControlWindow::updatePosition);

    header.setup("header");
    header.setLayout(ofxBaseGui::Horizontal);
    header.setShowHeader(false);
    header.setBorderColor(ofColor(0,0,0,0));

    title.setup("title","crawling traces", config_header);
    title.setShowName(false);
    header.add(title);
    header.add<ofxFpsPlotter>(config_header);
    //status.setup("STATUS");

    ofxBaseGui::Config config_header_btn = config_header;
    config_header_btn.shape.width = 0;

    save_settings_btn.addListener(this, &ControlWindow::saveAllSettings);
    save_settings_btn.setBackgroundColor(ofColor(0,0,0,0));
    header.addSpacer(13);
    header.add(save_settings_btn.setup("Save all settings", config_header_btn));
    header.add(recording.set("recording", false));
    recording.addListener(video_recorder_controller, &VideoRecorderController::setRecording);

//    import_events_btn.addListener(this, &ControlWindow::importGroup);
//    import_events_btn.setup("Import events");
//    gui.add(import_events_btn);

    //status.add(status_temp.set("Hardware temperatures", ".. showing status .."));

    server.setup(traces_controller);
    paths.setup(paths_controller);

    //setup mapping view
    mapping_controller->getMapping()->getControlView()->setGroupConfig(CustomTab().group_config);
    mapping_controller->getMapping()->getControlView()->setSliderConfig(CustomTab().slider_config);
    mapping_controller->getMapping()->getControlView()->setToggleConfig(CustomTab().toggle_config);
    mapping_controller->getMapping()->getControlView()->setLabelConfig(CustomTab().label_config);
    mapping_controller->getMapping()->getControlView()->setup(0,0,800,600);

    gui.setup("tabbed page");

    gui.setShowHeader(false);
    gui.setTabHeight(50);
    gui.setTabWidth(100);
    gui.setBackgroundColor(ofColor(22));
    gui.setBorderColor(ofColor(0,0,0,0));
    gui.setSize(w,h-header.getHeight()-10);
    gui.add(server);
    gui.add(paths);
    gui.add(*mapping_controller->getMapping()->getControlView());
    gui.setSize(w,h-header.getHeight()-10);

    addSpacer(10);
    add(header);
    addSpacer(10);
    add(gui);

    is_setup = true;

}

void ControlWindow::update() {

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
    mapping_controller->controlLeft().removeListener(this, &ControlWindow::updatePosition);
    save_settings_btn.removeListener(this, &ControlWindow::saveAllSettings);
    recording.removeListener(video_recorder_controller, &VideoRecorderController::setRecording);
}

void ControlWindow::saveAllSettings() {
    mapping_controller->getMapping()->getControl()->saveMappingDefault();
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
