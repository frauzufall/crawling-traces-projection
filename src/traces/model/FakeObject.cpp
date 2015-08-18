#include "FakeObject.h"
#include "ObjectController.h"
#include "MappingController.h"
#include "Traces.h"
#include "Stuff.h"

#include <algorithm>

using namespace guardacaso;

FakeObject::FakeObject(string future, string id):LightObject() {

    future_line.clear();
    future_timestamp.clear();

    stringstream future_svg, future_xml;

    future_svg << future << ".svg";
    future_xml << future << ".xml";

    ofxSVG_ptr svg = ofxSVG_ptr(new ofxSVG());
    svg->load(future_svg.str());

    if(svg->getNumPath() == 1) {
        future_line = svg->getPathAt(0).getOutline().at(0);
        color = svg->getPathAt(0).getStrokeColor();
    }
    else {
        cout << "DRAWINGOBJECT:: imported future with strange pathnum = " << svg->getNumPath() << endl;
    }

    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(future_xml.str()) ){

        xml->pushTag("timestamps");

            int stamps = xml->getNumTags("p");

            for(int i = 0; i < stamps; i++) {
                string timestr = xml->getValue("p","",i);
//                timestr.erase(std::remove(timestr.begin(), timestr.end(), '-'), timestr.end());
                //std::replace( timestr.begin(), timestr.end(), "-", "");
                future_timestamp.push_back(timestr.c_str());
            }

        xml->popTag();

    } else{
        cout << "unable to load xml file " << future_xml << endl;
    }
    if(future_line.size() == future_timestamp.size()) {
        cout << "DRAWINGOBJECT:: successfully imported future " << future << endl;
    }
    else {
        cout << "DRAWINGOBJECT:: error while importing future " << future
             << ": size of path is " << future_line.size()
             << " while size of timestamps is " << future_timestamp.size() << endl;
    }

    _id = id;
    if(future_line.size()>0)
        pos = future_line.getVertices().at(0);
    range = 0;
    backup_line.clear();
    line.clear();
    history_line.clear();
    history_timestamp.clear();
    history_net_line.clear();
    history_net_timestamp.clear();

}

void FakeObject::update(float range_max, float speed) {

    range = 100*range_max;
    this->speed = speed;

}

bool FakeObject::setPos(string timestamp, ofPoint p) {

    pos = p;

    backup_line.addVertex(pos);
    history_line.addVertex(pos);
    history_timestamp.push_back(timestamp);
    history_net_line.addVertex(pos);
    history_net_timestamp.push_back(timestamp);
    line.addVertex(pos);

    //check if point matches one of the line points other than the last one
    //if this is the case, the point was already added and no connection lines need to be drawn (they are already present)
    bool point_in_line = false;
    for(uint i = 0; i < backup_line.getVertices().size()-1; i++) {
       if(pos.get() == backup_line.getVertices().at(i)) {
           point_in_line = true;
           break;
       }
    }
    return point_in_line;

}

void FakeObject::addIntersection(string timestamp, ofPoint p){
    line.addVertex(p);
    line.addVertex(pos);
    history_net_line.addVertex(p);
    history_net_timestamp.push_back(timestamp);
    history_net_line.addVertex(pos);
    history_net_timestamp.push_back(timestamp);
}

ofColor FakeObject::getModColor() {
    return color;
}

ofPolyline FakeObject::getLine() {
    return backup_line;
}

ofPolyline FakeObject::getConnections() {
    return line;
}

FakeObject::~FakeObject() {}

void FakeObject::closeAndSave(string history_dir, float output_w, float output_h){
    cout << "DRAWINGOBJECT::saving fake object " << getId() << "..." << endl;
    stringstream svg_path, timestamp_path, svg_net_path, timestamp_net_path;
    svg_path << history_dir << "/" << getId() << ".svg";
    svg_net_path << history_dir << "/net_" << getId() << ".svg";
    timestamp_path << history_dir << "/" << getId() << ".xml";
    timestamp_net_path << history_dir << "/net_" << getId() << ".xml";

    Stuff::saveLineAsSvg(
                svg_path.str(),
                history_line,
                output_w,
                output_h,
                getColor());
    Stuff::saveLineAsSvg(
                svg_net_path.str(),
                history_net_line,
                output_w,
                output_h,
                getColor());
    saveTimestamps(timestamp_path.str());
    saveNetTimestamps(timestamp_net_path.str());
}

void FakeObject::saveTimestamps(string path) {
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

void FakeObject::saveNetTimestamps(string path) {
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

bool FakeObject::hasFuture() {
    return future_line.size()>0;
}

futureStepData FakeObject::getFutureStep(){
    futureStepData data;
    data.pos = future_line.getVertices().at(0);
    data.timestamp = future_timestamp.at(0);
    return data;

}

void FakeObject::removeFutureStep() {
    future_line.getVertices().erase(future_line.getVertices().begin());
    future_timestamp.erase(future_timestamp.begin());
}

void FakeObject::addHistoryNetLine(ofPoint p1, ofPoint p2, string timestamp) {

    bool double_line = false;

    for(uint i = 1; i < history_net_line.size()-1; i++) {

        ofPoint p = history_net_line[i];
        ofPoint p_prev = history_net_line[i-1];
        ofPoint p_next = history_net_line[i+1];

        if(p == p1) {
            if(p2 == p_prev || p2 == p_next) {
                double_line = true;
                break;
            }
        }

    }

    if(!double_line) {
        history_net_line.addVertex(p1);
        history_net_timestamp.push_back(timestamp);
        history_net_line.addVertex(p2);
        history_net_timestamp.push_back(timestamp);
    }
}
