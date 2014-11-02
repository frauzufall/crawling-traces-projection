#include "Traces.h"
#include "ServerController.h"
#include "ObjectController.h"
#include "Stuff.h"
#include "Visuals.h"

using namespace guardacaso;

Traces& Traces::get() {
    static Traces instance;
    return instance;
}

Traces::Traces() {
    xml_server = "sessions/last/server.xml";
    stringstream dummy_name;
    dummy_name << "walls_" << ofGetTimestampString();
    //string name = xml->getValue("clientname", dummy_name);

    client_id = dummy_name.str();

    stringstream history_dir_sstr;

    history_dir_sstr << "history/" << client_id;
    history_dir = history_dir_sstr.str();

    ofDirectory::createDirectory(history_dir);
}

void Traces::setup() {
    ObjectController::getInstance().setup();
    setupServer();
}

void Traces::setup(string ip, int port) {
    ObjectController::getInstance().setup();
    setupServer(ip, port);
}

void Traces::update() {
    ServerController::getInstance().update();
    ObjectController::getInstance().update();
}

void Traces::setupServer(string ip, int port){
    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(xml_server) ){
        reloadServer(xml, ip, port);
    } else{
        cout << "unable to load xml file " << xml_server << endl;
    }
	
}

void Traces::setupServer(){
    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(xml_server) ){
        reloadServer(xml);
    } else{
        cout << "unable to load xml file " << xml_server << endl;
    }

}

void Traces::reloadServer(ofxXmlSettings_ptr xml, string ip, int port) {

    xml->pushTag("traces", 0);

        bool active = xml->getAttribute("server", "active", (int)false);
        ServerController::getInstance().setActive(active);

        ServerController::getInstance().setup(ip,port,client_id);

        ObjectController::getInstance().setDrawingRangeMax(xml->getValue("range_max",0.3));
        ObjectController::getInstance().setDrawingRangeMin(xml->getValue("range_min",0.3));
        ObjectController::getInstance().setConnectToItself(xml->getValue("connect_itself",0));
        ObjectController::getInstance().setConnectToOthers(xml->getValue("connect_others",1));
        ObjectController::getInstance().setMaxConnections(xml->getValue("max_connect",10));
        ObjectController::getInstance().setDrawingSpeed(xml->getValue("speed",0.3));
        ObjectController::getInstance().setPulseTime(xml->getValue("pulsetime",0.2));
        ObjectController::getInstance().setMaxLines(xml->getValue("maxlines",0.5));
        ObjectController::getInstance().setFadeoutTimeIdle(xml->getValue("fadeout-idle",0.5));
        ObjectController::getInstance().setFadeoutTimeGone(xml->getValue("fadeout-gone",0.5));

    xml->popTag();

}

void Traces::reloadServer(ofxXmlSettings_ptr xml) {

    xml->pushTag("traces", 0);
    xml->pushTag("server", 0);

        string ip = xml->getValue("ip", "0.0.0.0");
        int port = xml->getValue("port", 41234);

    xml->popTag();
    xml->popTag();

    reloadServer(xml, ip, port);

}

void Traces::saveServer() {

    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("traces");

    xml.pushTag("traces", 0);

        xml.addTag("server");

        xml.addAttribute("server", "active", (int)ServerController::getInstance().getActive().get(),0);

        xml.pushTag("server", 0);

            xml.addValue("ip", ServerController::getInstance().getIp());
            xml.addValue("port", ServerController::getInstance().getPort());

        xml.popTag();

        xml.addValue("speed", ObjectController::getInstance().getDrawingSpeed());
        xml.addValue("range_max", ObjectController::getInstance().getDrawingRangeMax());
        xml.addValue("range_min", ObjectController::getInstance().getDrawingRangeMin());
        xml.addValue("connect_itself", ObjectController::getInstance().getConnectToItself());
        xml.addValue("connect_others", ObjectController::getInstance().getConnectToOthers());
        xml.addValue("max_connect", ObjectController::getInstance().getMaxConnections());
        xml.addValue("pulsetime", ObjectController::getInstance().getPulseTime());
        xml.addValue("maxlines", ObjectController::getInstance().getMaxLines());
        xml.addValue("fadeout-idle", ObjectController::getInstance().getFadeoutTimeIdle());
        xml.addValue("fadeout-gone", ObjectController::getInstance().getFadeoutTimeGone());

    xml.popTag();

    xml.saveFile(xml_server);

}


string Traces::clientId() {
    return client_id;
}

string Traces::historyDir() {
    return history_dir;
}

Traces::~Traces() {
}

void Traces::simulateGroup(string group_dir) {

    //find events in group

    ofDirectory group_folder(group_dir);
    group_folder.listDir();
    vector<ofFile> group_files = group_folder.getFiles();

    for(uint i = 0; i < group_files.size(); i++) {

        ofFile group_file = group_files.at(i);

        if(group_file.isDirectory()) {

            //found event

            //load mapping

            stringstream mapping_svg;
            mapping_svg << group_file.getAbsolutePath() << "/" << "mapping.svg";
            cout << "traces::trying to import " << mapping_svg.str() << endl;
            Visuals::get().importSvg(mapping_svg.str());

            //find users (=svgs with fitting xml)

            ofDirectory event_folder(group_file.getAbsolutePath());
            event_folder.allowExt("svg");
            event_folder.listDir();

            vector<ofFile> event_files = event_folder.getFiles();
            for(uint i = 0; i < event_files.size(); i++) {
                string user = ((ofFile)event_files.at(i)).getBaseName();
                stringstream user_path, xml;
                user_path << event_folder.getAbsolutePath() << "/" << user;
                xml << user_path.str() << ".xml";
                if(ofFile(xml.str()).exists()) {
                    ObjectController::getInstance().addFakeObj(user_path.str(), user);
                }
            }
        }
    }
}

void Traces::simulateGroups() {
    simulateGroup("history_processing/2013-07 Geschwistertreffen");
}
