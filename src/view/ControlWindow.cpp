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

	setName("crawling traces");
	setShowHeader(false);
	setBorderColor(ofColor::fromHex(0x4BB3A5));

	xml_gui = "sessions/last/gui.xml";

	ofJson floatleft = {{"float", "left"}};

	w = mapping_controller->getMapping()->getControl()->controlWidth();
	h = mapping_controller->getMapping()->getControl()->controlHeight();
	setSize(w,h);

	//system_temp_freq = 30000;
	//last_temp_update = -system_temp_freq;
	//status_temp = "";

//	ofxBaseGui::Config config_header;
//	config_header.backgroundColor = ofColor(0,0,0,0);
//	config_header.shape.width = 170;
//	config_header.shape.height = 30;

	bool left = mapping_controller->controlLeft().get();
	updatePosition(left);
	mapping_controller->controlLeft().addListener(this, &ControlWindow::updatePosition);

	addSpacer(ofJson({{"width", 10}, {"height", 10}, {"float", "left"}}));
	header = addGroup("header");
	header->setShowHeader(false);
	header->setBorderColor(ofColor(0,0,0,0));

	header->add(title.set("crawling traces"), floatleft);
	header->addFpsPlotter(ofJson({{"float", "left"}}));
	//status.setup("STATUS");

	save_settings.addListener(this, &ControlWindow::saveAllSettings);
	header->addSpacer(ofJson({{"width", 13}, {"height", 13}, {"float", "left"}}));
	header->add(save_settings.set("Save all settings"), floatleft);
	header->add(recording.set("recording", false), floatleft);
	recording.addListener(video_recorder_controller, &VideoRecorderController::setRecording);

//    import_events_btn.addListener(this, &ControlWindow::importGroup);
//    import_events_btn.setup("Import events");
//    gui.add(import_events_btn);

	//status.add(status_temp.set("Hardware temperatures", ".. showing status .."));

	//setup mapping view
//    mapping_controller->getMapping()->getControlView()->setGroupConfig(CustomTab().group_config);
//    mapping_controller->getMapping()->getControlView()->setSliderConfig(CustomTab().slider_config);
//    mapping_controller->getMapping()->getControlView()->setToggleConfig(CustomTab().toggle_config);
//    mapping_controller->getMapping()->getControlView()->setLabelConfig(CustomTab().label_config);
//	mapping_controller->getMapping()->getControlView()->setup(0,0,800,600);

	addSpacer(ofJson({{"width", 10}, {"height", 10}, {"float", "left"}}));
	tabbed_pages = addTabs("tabbed page");

	tabbed_pages->setShowHeader(false);
	tabbed_pages->setTabHeight(50);
	tabbed_pages->setTabWidth(100);
	tabbed_pages->setBackgroundColor(ofColor(22));
	tabbed_pages->setBorderColor(ofColor(0,0,0,0));
	tabbed_pages->setWidth(w);
	server = tabbed_pages->add<ServerTab>();
	paths = tabbed_pages->add<PathsTab>();
	mapping_controller->getMapping()->addControlViewTo(tabbed_pages);
//	tabbed_pages.setSize(w,h-header.getHeight()-10);

	server->setup(traces_controller);
	paths->setup(paths_controller);

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
	save_settings.removeListener(this, &ControlWindow::saveAllSettings);
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
