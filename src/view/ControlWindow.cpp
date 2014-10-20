#include "ControlWindow.h"
#include "MappingController.h"
#include "ServerController.h"
#include "ObjectController.h"
#include "PathsController.h"
#include "Stuff.h"

#include "ServerTab.h"
#include "MappingTab.h"
#include "PathsTab.h"

#include "Visuals.h"

#include <regex>
#include <iostream>
#include <string>

using namespace guardacaso;

ControlWindow& ControlWindow::getInstance() {
    static ControlWindow instance;
    return instance;
}

ControlWindow::ControlWindow() {
    is_setup = false;
}

bool ControlWindow::isSetup() {
    return is_setup;
}

void ControlWindow::setup() {

    font = "fonts/Ubuntu-L.ttf";
    xml_gui = "sessions/last/gui.xml";

    system_temp_freq = 30000;
    last_temp_update = -system_temp_freq;

    header_height = 0.19;
    w = Visuals::get().controlWidth();
    h = Visuals::get().controlHeight();
    x = 0;
    y = 0;

    status_temp = "";

    setGui();

    is_setup = true;

}

void ControlWindow::update() {

    w = Visuals::get().controlWidth();
    h = Visuals::get().controlHeight();

    mapping.update();
    paths.update();
    server.update();

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

void ControlWindow::reloadGui() {

    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();

    ofPoint mapping_pos(gui.getPosition().x, gui.getPosition().y+gui.getHeight()+10);
    ofPoint server_pos(gui.getPosition().x, gui.getPosition().y+gui.getHeight()+10);
    ofPoint paths_pos(gui.getPosition().x, gui.getPosition().y+gui.getHeight()+10);
//    ofPoint status_pos(gui.getPosition().x+gui.getWidth()+10, gui.getPosition().y);

    if( xml->loadFile(xml_gui) ){

        xml->pushTag("gui", 0);

            mapping_pos.x = xml->getAttribute("mapping", "x", mapping_pos.x);
            mapping_pos.y = xml->getAttribute("mapping", "y", mapping_pos.y);
            mapping.setVisible(xml->getAttribute("mapping", "visible", 1));

            server_pos.x = xml->getAttribute("server", "x", server_pos.x);
            server_pos.y = xml->getAttribute("server", "y", server_pos.y);
            server.setVisible(xml->getAttribute("server", "visible", 1));

            paths_pos.x = xml->getAttribute("paths", "x", paths_pos.x);
            paths_pos.y = xml->getAttribute("paths", "y", paths_pos.y);
            paths.setVisible(xml->getAttribute("paths", "visible", 1));

//            status_pos.x = xml->getAttribute("status", "x", status_pos.x);
//            status_pos.y = xml->getAttribute("status", "y", status_pos.y);
//            status.setVisible(xml->getAttribute("status", "visible", 1));

        xml->popTag();

    }else{
        cout << "unable to load xml file " << xml_gui << endl;
    }

//    status.setPlacing(ofRectangle(status_pos,0,0));
    server.setPlacing(ofRectangle(server_pos,0,0));
    paths.setPlacing(ofRectangle(paths_pos,0,0));
    mapping.setPlacing(ofRectangle(mapping_pos,0,0));

}

void ControlWindow::saveGui() {

    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("gui");
    xml.pushTag("gui", 0);

    xml.addTag("mapping");
    xml.addAttribute("mapping", "x", mapping.getPlacing().x,0);
    xml.addAttribute("mapping", "y", mapping.getPlacing().y,0);
    xml.addAttribute("mapping", "visible", mapping.isVisible(),0);

    xml.addTag("server");
    xml.addAttribute("server", "x", server.getPlacing().x,0);
    xml.addAttribute("server", "y", server.getPlacing().y,0);
    xml.addAttribute("server", "visible", server.isVisible(),0);

    xml.addTag("paths");
    xml.addAttribute("paths", "x", paths.getPlacing().x,0);
    xml.addAttribute("paths", "y", paths.getPlacing().y,0);
    xml.addAttribute("paths", "visible", paths.isVisible(),0);

//    xml.addTag("status");
//    xml.addAttribute("status", "x", status.getPlacing().x,0);
//    xml.addAttribute("status", "y", status.getPlacing().y,0);
//    xml.addAttribute("status", "visible", status.isVisible(),0);

    xml.popTag();

    xml.saveFile(xml_gui);

}

void ControlWindow::draw(ofPoint p) {
    
    gui.draw(p);
//    status.draw(p);
    server.draw(p);
    paths.draw(p);
    mapping.draw(p);

}

void ControlWindow::setGui() {

    gui.setup("crawling traces");
//    status.setup("STATUS");

    gui.setBorderColor(ofColor::black);
//    status.setBorderColor(ofColor::black);

    save_gui_btn.addListener(this, &ControlWindow::saveGui);
    gui.add(save_gui_btn.setup("Save GUI"));

    save_settings_btn.addListener(this, &ControlWindow::saveAllSettings);
    gui.add(save_settings_btn.setup("Save all settings"));

//    import_events_btn.addListener(this, &ControlWindow::importGroup);
//    import_events_btn.setup("Import events");
//    gui.add(&import_events_btn);

    gui.add(server.visible.set("show server", false));
    gui.add(paths.visible.set("show paths", true));
    gui.add(mapping.visible.set("show mapping", false));

//    status.add(status_temp.set("Hardware temperatures", ".. showing status .."));

    reloadGui();

}

bool ControlWindow::drawingMapping() {
    return mapping.isVisible();
}

void ControlWindow::saveAllSettings() {
    Visuals::get().saveMappingDefault();
    Traces::get().saveServer();
    Visuals::get().savePaths();
}

void ControlWindow::keyPressed(int key) {
    switch(key) {
    case '1':
        server.setVisible(!server.isVisible());
        break;
    case '2':
        paths.setVisible(!paths.isVisible());
        break;
    case '3':
        mapping.setVisible(!mapping.isVisible());
        break;
    default:break;
    }
}

MappingTab& ControlWindow::getMapping() {
    return mapping;
}

void ControlWindow::importGroup() {
    Traces::get().simulateGroups();
    mapping.reload();
}
