#include "DrawingObject.h"
#include "ObjectController.h"
#include "MappingController.h"
#include "ServerController.h"
#include "SoundController.h"
#include "Traces.h"
#include "Stuff.h"

#include <algorithm>

using namespace guardacaso;

DrawingObject::DrawingObject():LightObject() {
}

void DrawingObject::setup(string id) {
    _id = id;
    if(id == "wheels") {
        setNewRandomColor();
    }

    please_redraw = false;
    backup_line.clear();
    line.clear();
    history_line.clear();
    history_timestamp.clear();
    history_net_line.clear();
    history_net_timestamp.clear();
}

void DrawingObject::update() {

}


bool DrawingObject::setPos(string timestamp, ofPoint p) {

    last_action = ofGetElapsedTimef();

    pos = p;

    backup_line.addVertex(pos);
    history_line.addVertex(pos);
    history_timestamp.push_back(timestamp);
    history_net_line.addVertex(pos);
    history_net_timestamp.push_back(timestamp);
    line.addVertex(pos);

    please_redraw = true;

    //check if point matches one of the line points other than the last one
    //if this is the case, the point was already added and no connection lines need to be drawn (they are already present)
    bool point_in_line = false;
    for(uint i = 0; i < backup_line.getVertices().size()-1; i++) {
       if(pos.get() == backup_line.getVertices().at(i)) {
           point_in_line = true;
           //cout << "point in line with id " << this->getId() << endl;
           break;
       }
    }
    return point_in_line;

}

void DrawingObject::addIntersection(string timestamp, ofPoint p){
    line.addVertex(p);
    line.addVertex(pos);
    history_net_line.addVertex(p);
    history_net_timestamp.push_back(timestamp);
    history_net_line.addVertex(pos);
    history_net_timestamp.push_back(timestamp);
    please_redraw = true;
}

void DrawingObject::setFinishingPos() {

    string timestamp = ofGetTimestampString();
    pos = last_pos;

    history_line.addVertex(pos);
    history_timestamp.push_back(timestamp);
    history_net_line.addVertex(pos);
    history_net_timestamp.push_back(timestamp);

}


void DrawingObject::clearLines() {
    line.clear();
    backup_line.clear();
}

void DrawingObject::setNewRandomColor() {
    getColor().set(ofColor::fromHsb(ofRandom(0,255),255,255));
}

bool DrawingObject::needsRedraw() {
    return please_redraw;
}

void DrawingObject::gotRedrawn() {
    please_redraw = false;
}

//void DrawingObject::setPulsing() {
    //pulsestart = ofGetElapsedTimef();
    //pulseval = 1;
    //send to server that client is pulsing
    //TODO fix
    //ServerController::getInstance().send(ServerController::getInstance().getClientName(), "pulsing", _id);
//}

ofParameter<float> &DrawingObject::getPulseStart() {
    return pulsestart;
}

ofParameter<float> &DrawingObject::getPulseVal() {
    return pulseval;
}

ofParameter<float> &DrawingObject::getPulseDuration() {
    return pulseduration;
}

ofParameter<ofColor> &DrawingObject::getModColor() {
    return color;
}

ofPolyline& DrawingObject::getLine() {
    return backup_line;
}

ofPolyline DrawingObject::getConnections() {
    return line;
}

DrawingObject::~DrawingObject() {
}

void DrawingObject::saveTimestamps(string path) {
    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("timestamps");
    xml.pushTag("timestamps", 0);

    for(uint i = 0; i < history_timestamp.size(); i++) {
        xml.addValue("p",history_timestamp.at(i));
    }

    xml.popTag();

    xml.saveFile(path);
}

void DrawingObject::saveNetTimestamps(string path) {
    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("timestamps");
    xml.pushTag("timestamps", 0);

    for(uint i = 0; i < history_net_timestamp.size(); i++) {
        xml.addValue("p",history_net_timestamp.at(i));
    }

    xml.popTag();

    xml.saveFile(path);
}

void DrawingObject::closeAndSave(string history_dir, float output_w, float output_h) {

    if(getLine().size() > 0) {
        this->setFinishingPos();

        stringstream unique_id, test_path, svg_path, timestamp_path, svg_net_path, timestamp_net_path;

        int file_count = 1;
        unique_id << getId();
        test_path << history_dir << "/" << unique_id.str() << ".svg";
        while(ofFile::doesFileExist(test_path.str())) {
            unique_id.str("");
            test_path.str("");
            unique_id << getId() << "_" << file_count;
            test_path << history_dir << "/" << unique_id.str() << ".svg";
            ++file_count;
        }

        svg_path << history_dir << "/" << unique_id.str() << ".svg";
        svg_net_path << history_dir << "/net_" << unique_id.str() << ".svg";
        timestamp_path << history_dir << "/" << unique_id.str() << ".xml";
        timestamp_net_path << history_dir << "/net_" << unique_id.str() << ".xml";

        cout << "DRAWINGOBJECT::saving object " << unique_id.str() << "..." << endl;

        Stuff::saveLineAsSvg(
                    svg_path.str(),
                    history_line,
                    output_w,
                    output_h,
                    getColor());
        //Stuff::saveLineAsSvg(svg_net_path.str(),history_net_line,Visuals::get().outputWidth(), Visuals::get().outputHeight(),getColor());
        saveTimestamps(timestamp_path.str());
        //saveNetTimestamps(timestamp_net_path.str());
    }

}
