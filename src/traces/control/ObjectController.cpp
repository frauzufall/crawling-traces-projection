#include "ObjectController.h"
#include "ServerController.h"
#include "Visuals.h"
#include "MappingController.h"

using namespace guardacaso;

ObjectController &ObjectController::getInstance() {
    static ObjectController instance;
    return instance;
}

ObjectController::ObjectController() {
    draw_obj.clear();
    clients.clear();
    fake_objs.clear();
}

void ObjectController::setup() {
    client_speed = 0.3;
    wheel_speed = 0.7;
    client_range_max = 0.6;
    wheel_range = 0.1;
    wheel_range = client_range_max;
    max_lines = 100000;
    max_lines_percent = 0.5;
    pulse_time_percent = 0.2;
    max_pulse_time = 13;
    fadeout_time_idle = 0.005;
    fadeout_time_gone = 0.005;
    max_fadeout_time = 3600;
    connect_to_itself = false;
    connect_to_others = true;
}

void ObjectController::update() {

    // remove client lines if there are too many

    int lc = getTotalClientLineCount();
    if(lc > getMaxLinecount()) {
        DrawingObject_ptr inactive = getMostInactiveClient();
        if(inactive) {
            inactive->closeAndSave();
            inactive->clearLines();
            if(inactive->getId()=="wheels") {
                inactive->setNewRandomColor();
            }
        }
    }

    // TODO set pulsing clients size

    float min_pulse_time = 100000;

    //get colors for clients who are white because they were connected before of was connected
    for(uint i = 0; i<draw_obj.size(); i++ ) {

        DrawingObject_ptr obj = draw_obj.at(i);

        if(obj) {

            if(!obj->getFadingOut()) {
//                if(!obj->isGone()) {
                    //obj has connection
                    obj->update();
                    if(obj->getColor() == ofColor(255)) {
                        ServerController::getInstance().askForColor(obj->getId());
                    }
                    float pulsetime = obj->getPulseStart()-obj->getPulseVal();
                    if(pulsetime < min_pulse_time) {
                        pulsing_obj = obj;
                        min_pulse_time = pulsetime;
                    }
                    //check if client is not doing anything and, if so, blend out
                    ofColor col = obj->getColor();
                    float limit = fadeout_time_idle*max_fadeout_time;
                    float fadeout = min(limit,obj->getTimeIdle())/limit;
                    if(fadeout < 1) {
                        col.set(col.r,col.g,col.b, 255-255.0*fadeout);
                        obj->setColor(col);
                    }
                    else {
                        //obj->closeAndSave();
                        //obj->setGone(true);
                        removeClient(obj->getId());
                    }
//                }
            }
            else {
                //obj is gone
                float limit = fadeout_time_gone*max_fadeout_time;
                float fadeout = min(limit,obj->getTimeGone())/limit;
                if(fadeout < 1) {
                    //fade out
                    ofColor col = obj->getColor();
                    col.set(col.r,col.g,col.b, 255-255.0*fadeout);
                    obj->setColor(col);
                }
                else {
                    //obj->closeAndSave();
                    //obj->setGone(true);
                    removeClient(obj->getId());
                }
            }
        }
    }

    while(fake_objs.size()>0) {
        for(uint i = 0; i<fake_objs.size(); i++ ) {

            FakeObject_ptr obj = fake_objs.at(i);

            if(obj) {

                if(obj->hasFuture()) {
                    obj->futureStep();
                    obj->update();
                }
                else {
                    fake_objs.erase(fake_objs.begin()+i);
                }
            }
        }
    }
}

map<string, DrawingObject_ptr> ObjectController::getClients() {
    return clients;
}

DrawingObject_ptr ObjectController::getClient(string id) {

    map<string,DrawingObject_ptr>::iterator it = clients.find(id);

    if(it == clients.end()) {
        cout << "OBJECTCONTROLLER: new client " << id << endl;
        DrawingObject_ptr c = DrawingObject_ptr(new DrawingObject());
        c->setup(id);
        clients[id] = c;
        draw_obj.push_back(c);
    }

    return clients[id];
}

FakeObject_ptr ObjectController::addFakeObj(string path, string id) {

    cout << "OBJECTCONTROLLER: new fake object" << id << endl;
    FakeObject_ptr c = FakeObject_ptr(new FakeObject(path, id));
    fake_objs.push_back(c);

    return c;
}

void ObjectController::removeObject(DrawingObject_ptr drawing_o) {

    if(pulsing_obj == drawing_o) {
        pulsing_obj.reset();
    }

    vector<DrawingObject_ptr>::iterator dr_obj_it = draw_obj.begin();
    while(dr_obj_it != draw_obj.end()) {
        if((*dr_obj_it) == drawing_o) {
            cout << "OBJECTCONTROLLER: gone object " << drawing_o->getName() << endl;
            draw_obj.erase(dr_obj_it);
            break;
        }
        else {
            ++dr_obj_it;
        }
    }
}

void ObjectController::fadeoutClient(string id) {
    map<string,DrawingObject_ptr>::iterator it = clients.find(id);
    if(it!=clients.end()) {
        DrawingObject_ptr obj = it->second;
        obj->setFadingOut(true);
        obj->setFadingOutStart(ofGetElapsedTimef());
    }
}

void ObjectController::deactivateClient(string id) {
    getClient(id)->setGone(true);
}

void ObjectController::activateClient(string id) {
    getClient(id)->setGone(false);
}

void ObjectController::removeClient(string id) {
    map<string,DrawingObject_ptr>::iterator it = clients.find(id);
    if(it!=clients.end())
        removeClient(it);
}

void ObjectController::removeClient(map<string,DrawingObject_ptr>::iterator it) {
    removeObject(it->second);
    string id = it->second->getId();
    if(it != clients.end()) {
        cout << "OBJECTCONTROLLER: gone client " << it->second->getId() << endl;
        clients.erase(it);
    }
}


void ObjectController::removeAllClients() {
    while( !clients.empty() ) {
        removeClient(clients.begin());
    }
}

int ObjectController::getMaxLinecount() {
    return max_lines*max_lines_percent;
}

int ObjectController::getTotalClientLineCount() {
    int res = 0;
    map<string,DrawingObject_ptr>::iterator iter;
    DrawingObject_ptr c;
    for( iter = clients.begin(); iter != clients.end(); iter++ ) {

        c = iter->second;

        if(c) {

            res+=c->getConnections().size();

        }
    }
    return res;
}

DrawingObject_ptr ObjectController::getMostInactiveClient() {
    float max_inactive = ofGetElapsedTimef();
    map<string,DrawingObject_ptr>::iterator iter;
    DrawingObject_ptr c;
    DrawingObject_ptr c_last;
    for( iter = clients.begin(); iter != clients.end(); iter++ ) {

        c = iter->second;

        if(c) {

            if(c->getLastActionTime()<max_inactive && c->getConnections().size()>0) {
                max_inactive = c->getLastActionTime();
                c_last = c;
            }

        }
    }
    return c_last;
}

ofParameter<float> ObjectController::getDrawingRangeMax() {
    return client_range_max;
}

void ObjectController::setDrawingRangeMax(float range) {
    client_range_max = range;
}

ofParameter<float> ObjectController::getDrawingRangeMin() {
    return client_range_min;
}

void ObjectController::setDrawingRangeMin(float range) {
    client_range_min = range;
}

ofParameter<float> ObjectController::getDrawingRangeWheels() {
    return wheel_range;
}

void ObjectController::setDrawingRangeWheels(float range) {
    wheel_range = range;
}

ofParameter<float> ObjectController::getDrawingSpeed() {
    return client_speed;
}

void ObjectController::setDrawingSpeed(float speed) {
    if(speed <0.01)
        speed = 0.01;
    client_speed = speed;
}

ofParameter<float> ObjectController::getDrawingSpeedWheels() {
    return wheel_speed;
}

void ObjectController::setDrawingSpeedWheels(float speed) {
    if(speed <0.01)
        speed = 0.01;
    wheel_speed = speed;
}

ofPoint ObjectController::getCurrentPulsingPoint() {
    ofPoint res;
    if(pulsing_obj) {
        res = pulsing_obj->getPos();
    }
    else {
        res.x = MappingController::getInstance().getProjector(0)->getStartPoint().x*Visuals::get().contentWidth();
        res.y = MappingController::getInstance().getProjector(0)->getStartPoint().y*Visuals::get().contentHeight();
    }
    return res;
}

ofParameter<float> ObjectController::getPulseTime() {
    return pulse_time_percent;
}

float ObjectController::getMaxPulseTime() {
    return max_pulse_time;
}

void ObjectController::setPulseTime(float time) {
    pulse_time_percent = time;
}

ofParameter<float> ObjectController::getMaxLines() {
    return max_lines_percent;
}

void ObjectController::setMaxLines(float percentage) {
    max_lines_percent = percentage;
}

ofParameter<float> ObjectController::getFadeoutTimeIdle() {
    return fadeout_time_idle;
}

void ObjectController::setFadeoutTimeIdle(float time) {
    fadeout_time_idle = time;
}

ofParameter<float> ObjectController::getFadeoutTimeGone() {
    return fadeout_time_gone;
}

void ObjectController::setFadeoutTimeGone(float time) {
    fadeout_time_gone = time;
}

ofParameter<float> ObjectController::getMaxFadeoutTime() {
    return max_fadeout_time;
}

vector<DrawingObject_ptr> ObjectController::getDrawingObjects() {
    return draw_obj;
}

void ObjectController::setConnectToItself(bool connect) {
    connect_to_itself = connect;
}

ofParameter<bool> ObjectController::getConnectToItself() {
    return connect_to_itself;
}

void ObjectController::setConnectToOthers(bool connect) {
    connect_to_others = connect;
}

ofParameter<bool> ObjectController::getConnectToOthers() {
    return connect_to_others;
}

void ObjectController::setMaxConnections(int num) {
    max_connections = num;
}

ofParameter<int> ObjectController::getMaxConnections() {
    return max_connections;
}
