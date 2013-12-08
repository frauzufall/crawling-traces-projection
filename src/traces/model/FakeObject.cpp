#include "FakeObject.h"
#include "ObjectController.h"
#include "MappingController.h"
#include "Traces.h"
#include "Stuff.h"

#include <algorithm>

using namespace guardacaso;

FakeObject::FakeObject(string future, string id) {

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


void FakeObject::update() {

    range = 100*ObjectController::getInstance().getDrawingRange();

    speed = ObjectController::getInstance().getDrawingSpeed();

}

void FakeObject::setPos(ofPoint p, string timestamp) {

    int max_distance = range;

    last_pos = pos;

    pos = p;

    backup_line.addVertex(pos);
    history_line.addVertex(pos);
    history_timestamp.push_back(timestamp);
    history_net_line.addVertex(pos);
    history_net_timestamp.push_back(timestamp);
    line.addVertex(pos);
    for(uint i = 0; i < backup_line.getVertices().size(); i++) {
        ofPoint p_tmp = backup_line.getVertices().at(i);
        if(p_tmp.distance(pos) < max_distance) {
            line.addVertex(p_tmp);
            line.addVertex(pos);
            history_net_line.addVertex(p_tmp);
            history_net_timestamp.push_back(timestamp);
            history_net_line.addVertex(pos);
            history_net_timestamp.push_back(timestamp);
        }
    }

    backup_line.simplify();

    Projector* pj = MappingController::getInstance().getProjector(0);
    MappingQuad_ptr q;

    for(uint i = 0; i < pj->quadCount(); i++) {
        q = pj->getQuad(i);
        if(q) {
            ofPolyline polyline = Visuals::get().outlinesRaw()->at(i);
            if(q->nature == "window") {
                for(uint j = 0; j < polyline.size(); j++) {
                    vector<ofPoint> points_between = Stuff::getPointsBetween(polyline[j],
                                                                             polyline[(j+1)%polyline.size()],
                                                                             10);
                    for(uint k = 0; k < points_between.size(); k++) {
                        if(pos.distance(points_between[k]) < max_distance) {
                            line.addVertex(points_between[k]);
                            line.addVertex(pos);
                            history_net_line.addVertex(points_between[k]);
                            history_net_timestamp.push_back(timestamp);
                            history_net_line.addVertex(pos);
                            history_net_timestamp.push_back(timestamp);
                        }
                    }
                }
            }
        }
    }


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

FakeObject::~FakeObject() {
    cout << "DRAWINGOBJECT::saving fake object " << getId() << "..." << endl;
    stringstream svg_path, timestamp_path, svg_net_path, timestamp_net_path;
    svg_path << Traces::get().historyDir() << "/" << getId() << ".svg";
    svg_net_path << Traces::get().historyDir() << "/net_" << getId() << ".svg";
    timestamp_path << Traces::get().historyDir() << "/" << getId() << ".xml";
    timestamp_net_path << Traces::get().historyDir() << "/net_" << getId() << ".xml";
    Stuff::saveLineAsSvg(svg_path.str(),history_line,Visuals::get().outputWidth(), Visuals::get().outputHeight(),getColor());
    Stuff::saveLineAsSvg(svg_net_path.str(),history_net_line,Visuals::get().outputWidth(), Visuals::get().outputHeight(),getColor());
    saveTimestamps(timestamp_path.str());
    saveNetTimestamps(timestamp_net_path.str());
}

void FakeObject::saveTimestamps(string path) {
    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("timestamps");
    xml.pushTag("timestamps", 0);

    for(int i = 0; i < history_timestamp.size(); i++) {
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

    for(int i = 0; i < history_net_timestamp.size(); i++) {
        xml.addValue("p",history_net_timestamp.at(i));
    }

    xml.popTag();

    xml.saveFile(path);
}

bool FakeObject::hasFuture() {
    return future_line.size()>0;
}

void FakeObject::futureStep() {
    setPos(future_line.getVertices().at(0), future_timestamp.at(0));
    future_line.getVertices().erase(future_line.getVertices().begin());
    future_timestamp.erase(future_timestamp.begin());
}

void FakeObject::addHistoryNetLine(ofPoint p1, ofPoint p2, string timestamp) {

    bool double_line = false;

    for(int i = 1; i < history_net_line.size()-1; i++) {

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
