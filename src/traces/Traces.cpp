#include "Traces.h"
#include "Stuff.h"

using namespace guardacaso;

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

    object_controller.getHistoryDir().makeReferenceTo(history_dir);

}

void Traces::setup(MappingController *mc) {
    mapping_controller = mc;
    object_controller.setup(mc);
    setupServer();
}

void Traces::setup(MappingController* mc, string ip, int port) {
    mapping_controller = mc;
    object_controller.setup(mc);
    setupServer(ip, port);
}

void Traces::update() {
    server_controller.update();
    object_controller.update(&server_controller);
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
        server_controller.setActive(active);

        server_controller.setup(ip,port,client_id);

        object_controller.getDrawingRangeMax().set(xml->getValue("range_max",object_controller.getDrawingRangeMax()));
        object_controller.getDrawingRangeMin().set(xml->getValue("range_min",object_controller.getDrawingRangeMin()));
        object_controller.getConnectToItself().set(xml->getValue("connect_itself",object_controller.getConnectToItself()));
        object_controller.getConnectToOthers().set(xml->getValue("connect_others",object_controller.getConnectToOthers()));
        object_controller.getMaxConnections().set(xml->getValue("max_connect",object_controller.getMaxConnections()));
        object_controller.getDrawingSpeed().set(xml->getValue("speed",object_controller.getDrawingSpeed()));
        object_controller.getPulseTime().set(xml->getValue("pulsetime",object_controller.getPulseTime()));
        object_controller.getMaxLines().set(xml->getValue("maxlines",object_controller.getMaxLines()));
        object_controller.getFadeoutTimeIdle().set(xml->getValue("fadeout-idle",object_controller.getFadeoutTimeIdle()));
        object_controller.getFadeoutTimeGone().set(xml->getValue("fadeout-gone",object_controller.getFadeoutTimeGone()));

    xml->popTag();

    ofAddListener(server_controller.messageReceived, &object_controller, &ObjectController::processMsg);
    ofAddListener(object_controller.sendingPosition, &server_controller, &ServerController::sendPosition);
    ofAddListener(server_controller.mappingRequested, this, &Traces::sendMapping);

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

        xml.addAttribute("server", "active", (int)server_controller.getActive().get(),0);

        xml.pushTag("server", 0);

            xml.addValue("ip", server_controller.getIp());
            xml.addValue("port", server_controller.getPort());

        xml.popTag();

        xml.addValue("speed", object_controller.getDrawingSpeed());
        xml.addValue("range_max", object_controller.getDrawingRangeMax());
        xml.addValue("range_min", object_controller.getDrawingRangeMin());
        xml.addValue("connect_itself", object_controller.getConnectToItself());
        xml.addValue("connect_others", object_controller.getConnectToOthers());
        xml.addValue("max_connect", object_controller.getMaxConnections());
        xml.addValue("pulsetime", object_controller.getPulseTime());
        xml.addValue("maxlines", object_controller.getMaxLines());
        xml.addValue("fadeout-idle", object_controller.getFadeoutTimeIdle());
        xml.addValue("fadeout-gone", object_controller.getFadeoutTimeGone());

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
    ofRemoveListener(server_controller.messageReceived, &object_controller, &ObjectController::processMsg);
    ofRemoveListener(object_controller.sendingPosition, &server_controller, &ServerController::sendPosition);
    ofRemoveListener(server_controller.mappingRequested, this, &Traces::sendMapping);
}

void Traces::simulateGroup(MappingController *mc, string group_dir) {

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
            mc->getMapping()->getControl()->importSvg(mapping_svg.str());

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
                    object_controller.addFakeObj(user_path.str(), user);
                }
            }
        }
    }
}

void Traces::simulateGroups(MappingController* mc) {
    simulateGroup(mc, "history_processing/2013-07 Geschwistertreffen");
}

void Traces::sendMapping(bool &){
    getServerController()->sendMappingQuads(mapping_controller->getMapping()->getControl()->getProjector());
}

ServerController* Traces::getServerController(){
    return &server_controller;
}

ObjectController* Traces::getObjectController(){
    return &object_controller;
}
