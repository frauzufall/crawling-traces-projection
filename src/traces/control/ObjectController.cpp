#include "ObjectController.h"
#include "Stuff.h"

using namespace guardacaso;

ObjectController::ObjectController() {
    draw_obj.clear();
    clients.clear();
    fake_objs.clear();

    history_dir.set("history folder", "history");
    client_speed.set("client speed", 0.3, 0,1);
    wheel_speed.set("wheel speed", 2.1,0,3);
    client_range_max.set("client range max", 60,0,100);
    client_range_min.set("client range min", 10,0,100);
    wheel_range.set("wheel range", 10,0,100);
//    wheel_range = client_range_max;    //?!
    max_lines.set("max lines", 100000, 0, 100000);
    max_lines_percent.set("max lines percent", 0.5,0,1);
    pulse_time_percent.set("pulse line percent", 0.2,0,1);
    max_pulse_time.set("max pulse time", 13,0,100);
    fadeout_time_idle.set("fadeout time idle", 0.005,0,1);
    fadeout_time_gone.set("fadeout time gone",0.005,0,1);
    max_fadeout_time.set("max fadeout time", 36000, 0, 60000);
    connect_to_itself.set("connect to itself", false);
    connect_to_others.set("connect to others", true);
    max_connections.set("max connections",100,0,1000);
}

void ObjectController::setup(MappingController *mc) {
    mapping_controller = mc;
}

void ObjectController::update(ServerController *server_controller) {

    // remove client lines if there are too many

    int lc = getTotalClientLineCount();
    if(lc > getMaxLinecount()) {
        DrawingObject_ptr inactive = getMostInactiveClient();
        if(inactive) {
            inactive->closeAndSave(history_dir,
                mapping_controller->getMapping()->getControl()->getProjector()->outputWidth(),
                mapping_controller->getMapping()->getControl()->getProjector()->outputHeight());
            inactive->clearLines();
            if(inactive->getId().get()=="wheels") {
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
                    if(obj->getColor().get() == ofColor(255)) {
                        server_controller->askForColor(obj->getId());
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
                        obj->getColor().set(col);
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
                    obj->getColor().set(col);
                }
                else {
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
                    futureStepData data = obj->getFutureStep();
                    setFakeObjectPos(obj, data.pos, data.timestamp);
                    obj->removeFutureStep();
                    obj->update(getDrawingRangeMax(), getDrawingSpeed());
                }
                else {
                    obj->closeAndSave(history_dir,
                        mapping_controller->getMapping()->getControl()->getProjector()->outputWidth(),
                        mapping_controller->getMapping()->getControl()->getProjector()->outputHeight());
                    fake_objs.erase(fake_objs.begin()+i);
                }
            }
        }
    }
}

void ObjectController::processMsg(ctMessage& msg) {

   if(msg.client_id != "all") {

        if(msg.action == "gone") {
            deactivateClient(msg.client_id);
            fadeoutClient(msg.client_id);
        }
        else {
            DrawingObject_ptr c = getClient(msg.client_id);

            c->getIsGone().set(false);

            if(msg.action == "id") {
                c->getId().set(msg.value);
            }
            if(msg.action == "type") {
                c->getType().set(msg.value);
            }
            if(msg.action == "name") {
                c->getName().set(msg.value);
            }
            if(msg.action == "pos") {
                vector<string> pos = ofSplitString(msg.value,"|");
                setObjectPos(c, ofPoint(atof(pos[0].c_str()),atof(pos[1].c_str())));
            }
            if(msg.action == "pulse") {
                c->getPulseStart().set(ofGetElapsedTimef());
                c->getPulseVal().set(1);
                //send to server that client is pulsing
                ctMessage msg;
                msg.action = "pulsing";
                msg.value = c->getId();
                ofNotifyEvent(sendingPosition,msg,this);
            }
            if(msg.action == "color") {
                vector<string> col = ofSplitString(msg.value,"|");
                c->getColor().set(ofColor(atoi(col[0].c_str()),atoi(col[1].c_str()), atoi(col[2].c_str())));
            }
            if(msg.action == "clear") {
                c->closeAndSave(history_dir,
                                mapping_controller->getMapping()->getControl()->getProjector()->outputWidth(),
                                mapping_controller->getMapping()->getControl()->getProjector()->outputHeight());
                c->clearLines();
            }
        }

    }
    else {
        if(msg.action == "clear") {
            removeAllClients();
        }
    }
}

map<string, DrawingObject_ptr>& ObjectController::getClients() {
    return clients;
}

DrawingObject_ptr ObjectController::getClient(string id) {

    map<string,DrawingObject_ptr>::iterator it = clients.find(id);

    if(it == clients.end()) {
        cout << "OBJECTCONTROLLER: new client " << id << endl;

        DrawingObject_ptr c = addDrawingObject(id);

        clients[id] = c;
        draw_obj.push_back(c);
    }

    return clients[id];
}

DrawingObject_ptr ObjectController::addDrawingObject(string id){
    DrawingObject_ptr c = DrawingObject_ptr(new DrawingObject());
    c->setup(id);
    ofPoint start = getStartingPoint();
    start.x *= mapping_controller->getMapping()->getControl()->getProjector()->outputWidth();
    start.y *= mapping_controller->getMapping()->getControl()->getProjector()->outputHeight();
    c->getPos().set(start);

    //send starting position to server
    stringstream possent;
    ofx2DMappingProjector *p = mapping_controller->getMapping()->getControl()->getProjector(0);
    ofPoint posres = c->getPos();
    if(p->getUsingCam()) {
        posres = p->inCameraView(c->getPos());
    }
    possent << id << ";" << posres.x << "|" << posres.y;
    ctMessage msg;
    msg.action = "moveto";
    msg.value = possent.str();
    ofNotifyEvent(sendingPosition,msg,this);

    //start pulsing
    c->getPulseDuration().set(getMaxPulseTime()*getPulseTime());
    c->getPulseStart().set(ofGetElapsedTimef());
    c->getPulseVal().set(1);

    //send to server that client is pulsing
    ctMessage msg2;
    msg2.action = "pulsing";
    msg2.value = id;
    ofNotifyEvent(sendingPosition,msg2,this);

    return c;
}

FakeObject_ptr ObjectController::addFakeObj(string path, string id) {

    cout << "OBJECTCONTROLLER: new fake object" << id << endl;
    FakeObject_ptr c = FakeObject_ptr(new FakeObject(path, id));
    fake_objs.push_back(c);

    return c;
}

void ObjectController::removeObject(DrawingObject_ptr drawing_o) {

    drawing_o->closeAndSave(history_dir,
         mapping_controller->getMapping()->getControl()->getProjector()->outputWidth(),
         mapping_controller->getMapping()->getControl()->getProjector()->outputHeight());

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
        obj->getFadingOut().set(true);
        obj->getFadingOutStart().set(ofGetElapsedTimef());
    }
}

void ObjectController::deactivateClient(string id) {
    getClient(id)->getIsGone().set(true);
}

void ObjectController::activateClient(string id) {
    getClient(id)->getIsGone().set(false);
}

void ObjectController::removeClient(string id) {
    map<string,DrawingObject_ptr>::iterator it = clients.find(id);
    if(it!=clients.end())
        removeClient(it);
}

void ObjectController::removeClient(map<string,DrawingObject_ptr>::iterator it) {
    string id = it->second->getId();
    removeObject(it->second);
    if(it != clients.end()) {
        cout << "OBJECTCONTROLLER: gone client " << id << endl;
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

void ObjectController::setObjectPos(DrawingObject_ptr obj, ofPoint p) {

    //SoundController::getInstance().triggerActivity();

    ofPoint p_d = p;

    float max_distance = client_range_max;
    float min_distance = client_range_min;
    if(obj->getId().get() == "wheels"){
        max_distance = wheel_range;
    }

    obj->getLastPos().set(obj->getPos());
    float speed = client_speed;
    if(obj->getId().get() == "wheels"){
        speed = wheel_speed;
    }

    if(abs(p_d.x)>1)
        p_d.x *= speed;
    if(abs(p_d.y)>1)
        p_d.y *= speed;

    ofPoint new_pos = obj->getLastPos().get()+p_d;
    ofPoint new_pos_corrected = new_pos;
    if(new_pos != obj->getPos()) {
        new_pos_corrected = mapping_controller->getMapping()->getControl()->getPointInMappedArea(obj->getLastPos(),new_pos);
    }

//    if(new_pos_corrected != pos) {

    string timestamp = ofGetTimestampString();
    bool point_in_line = obj->setPos(timestamp, new_pos_corrected);

    ofPoint pos = new_pos_corrected;

    //send new position to server
    stringstream possent;

    ofx2DMappingProjector* pj = mapping_controller->getMapping()->getControl()->getProjector();

    ofPoint posres = pos;
    if(mapping_controller->getMapping()->getControl()->getProjector()->getUsingCam()) {
        posres = pj->inCameraView(pos);
    }
    possent << obj->getId() << ";" << posres.x << "|" << posres.y;
    //cout << "possent: " << possent.str() << endl;
    ctMessage msg;
    msg.action = "lineto";
    msg.value = possent.str();
    ofNotifyEvent(sendingPosition,msg,this);

    if(!point_in_line) {
        //create connection lines to existing drawing points
        int con_num = 0;
        if(connect_to_others) {
            for(uint j = 0; j < getDrawingObjects().size() && con_num < max_connections; j++) {
                if(getDrawingObjects().at(j)->getId().get() != obj->getId().get()) {
                    ofPolyline l = getDrawingObjects().at(j)->getLine();
                    for(uint i = 0; i < l.getVertices().size(); i++) {
                        ofPoint p_tmp = l.getVertices().at(i);
                        float distance = p_tmp.distance(pos);
                        if(distance < min_distance) {
                            continue;
                        }
                        if(distance < max_distance) {
                            //SoundController::getInstance().triggerActivity();
                            obj->addIntersection(timestamp, p_tmp);
                            con_num++;
                        }
                    }
                }
            }
        }
        if(connect_to_itself) {
            for(uint i = 0; i < obj->getLine().getVertices().size() && con_num < max_connections; i++) {
                ofPoint p_tmp = obj->getLine().getVertices().at(i);
                float distance = p_tmp.distance(pos);
                if(distance < min_distance) {
                    continue;
                }
                if(distance < max_distance) {
                   // SoundController::getInstance().triggerActivity();
                    obj->addIntersection(timestamp, p_tmp);
                    con_num++;
                }
            }
        }
    }

    obj->getLine().simplify();

    ofPtr<ofx2DMappingObject> mo;

    //add lines to borders of obstacles if they are nearby
    for(uint i = 0; i < pj->shapeCount(); i++) {
        mo = pj->getMappingObject(i);
        if(ofPtr<ofx2DMappingShape> shape = std::dynamic_pointer_cast<ofx2DMappingShape>(mo)){
            if(shape->name == "window") {
                ofPolyline polyline = mapping_controller->getMapping()->getControl()->getProjector(0)->outlinesRaw()->at(i);
                for(uint j = 0; j < polyline.size(); j++) {
                    vector<ofPoint> points_between = Stuff::getPointsBetween(polyline[j],
                                                                             polyline[(j+1)%polyline.size()],
                                                                             10);
                    for(uint k = 0; k < points_between.size(); k++) {
                        //cout << pos.distance(points_between[k]) << endl;
                        if(pos.distance(points_between[k]) < max_distance) {
                            //SoundController::getInstance().triggerActivity();
                            obj->addIntersection(timestamp, points_between[k]);
                        }
                    }
                }
            }
        }
    }


}

void ObjectController::setFakeObjectPos(FakeObject_ptr obj, ofPoint p, string timestamp) {

    bool point_in_line = obj->setPos(timestamp, p);

    int max_distance = client_range_max;
    int min_distance = client_range_min;
    if(obj->getId().get() == "wheels"){
        max_distance = wheel_range;
    }

    ofPoint pos = p;

    ofx2DMappingProjector* pj = mapping_controller->getMapping()->getControl()->getProjector();

    if(!point_in_line) {
        //create connection lines to existing drawing points
        int con_num = 0;
        if(connect_to_others) {
            for(uint j = 0; j < fake_objs.size() && con_num < max_connections; j++) {
                if(fake_objs.at(j)->getId().get() != obj->getId().get()) {
                    ofPolyline l = fake_objs.at(j)->getLine();
                    for(uint i = 0; i < l.getVertices().size(); i++) {
                        ofPoint p_tmp = l.getVertices().at(i);
                        float distance = p_tmp.distance(pos);
                        if(distance < min_distance) {
                            continue;
                        }
                        if(distance < max_distance) {
                            obj->addIntersection(timestamp, p_tmp);
                            con_num++;
                        }
                    }
                }
            }
        }
        if(connect_to_itself) {
            for(uint i = 0; i < obj->getLine().getVertices().size() && con_num < max_connections; i++) {
                ofPoint p_tmp = obj->getLine().getVertices().at(i);
                float distance = p_tmp.distance(pos);
                if(distance < min_distance) {
                    continue;
                }
                if(distance < max_distance) {
                    obj->addIntersection(timestamp, p_tmp);
                    con_num++;
                }
            }
        }
    }

    obj->getLine().simplify();

    ofPtr<ofx2DMappingObject> mo;

    //add lines to borders of obstacles if they are nearby
    for(uint i = 0; i < pj->shapeCount(); i++) {
        mo = pj->getMappingObject(i);
        if(ofPtr<ofx2DMappingShape> shape = std::dynamic_pointer_cast<ofx2DMappingShape>(mo)){
            if(shape->nature == "window") {
                ofPolyline polyline = mapping_controller->getMapping()->getControl()->getProjector(0)->outlinesRaw()->at(i);
                for(uint j = 0; j < polyline.size(); j++) {
                    vector<ofPoint> points_between = Stuff::getPointsBetween(polyline[j],
                                                                             polyline[(j+1)%polyline.size()],
                                                                             10);
                    for(uint k = 0; k < points_between.size(); k++) {
                        if(pos.distance(points_between[k]) < max_distance) {
                            //SoundController::getInstance().triggerActivity();
                            obj->addIntersection(timestamp, points_between[k]);
                        }
                    }
                }
            }
        }
    }

//    }
//    else {
//        pos = last_pos;
//    }

}


ofParameter<float> &ObjectController::getDrawingRangeMax() {
    return client_range_max;
}

ofParameter<float> &ObjectController::getDrawingRangeMin() {
    return client_range_min;
}

ofParameter<float> &ObjectController::getDrawingRangeWheels() {
    return wheel_range;
}

ofParameter<float> &ObjectController::getDrawingSpeed() {
    return client_speed;
}

ofParameter<float> &ObjectController::getDrawingSpeedWheels() {
    return wheel_speed;
}

//ofPoint ObjectController::getCurrentPulsingPoint() {
//    ofPoint res;
//    if(pulsing_obj) {
//        res = pulsing_obj->getPos();
//    }
//    else {
//        res.x = mapping_controller->getMapping()->getControl()->getProjector(0)->getStartPoint().x*Visuals::get().contentWidth();
//        res.y = mapping_controller->getMapping()->getControl()->getProjector(0)->getStartPoint().y*Visuals::get().contentHeight();
//    }
//    return res;
//}

ofParameter<float> &ObjectController::getPulseTime() {
    return pulse_time_percent;
}

float ObjectController::getMaxPulseTime() {
    return max_pulse_time;
}

ofParameter<float> &ObjectController::getMaxLines() {
    return max_lines_percent;
}

ofParameter<float> &ObjectController::getFadeoutTimeIdle() {
    return fadeout_time_idle;
}

ofParameter<float> &ObjectController::getFadeoutTimeGone() {
    return fadeout_time_gone;
}

ofParameter<float> &ObjectController::getMaxFadeoutTime() {
    return max_fadeout_time;
}

vector<DrawingObject_ptr> &ObjectController::getDrawingObjects() {
    return draw_obj;
}

ofParameter<bool> &ObjectController::getConnectToItself() {
    return connect_to_itself;
}

ofParameter<bool> &ObjectController::getConnectToOthers() {
    return connect_to_others;
}

ofParameter<int> &ObjectController::getMaxConnections() {
    return max_connections;
}

ofPoint ObjectController::getStartingPoint(){
    for(auto &e : mapping_controller->getMapping()->getControl()->getProjector()->getShapesByClass<ofx2DMappingPoint>()){
        if(e->name == "start"){
            return e->pos;
        }
    }
    ofPoint p;
    p.x = 0.5;
    p.y = 0.5;
    return p;
}

ofParameter<string>& ObjectController::getHistoryDir(){
    return history_dir;
}
