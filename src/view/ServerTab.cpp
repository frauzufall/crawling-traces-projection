#include "ServerTab.h"
#include "ObjectController.h"
#include "ServerController.h"
#include "Visuals.h"
#include "Traces.h"

using namespace guardacaso;

ServerTab::ServerTab():CustomTab() {

    setup("SERVER");

    setBorderColor(ofColor::black);
    setHeaderBackgroundColor(ofColor(235,212,0,0.5));

    save_btn.addListener(&Traces::get(), &Traces::saveServer);
    save_btn.setup("SAVE");
    add(&save_btn);

    add(ServerController::getInstance().getActive().set("ACTIVE", ServerController::getInstance().getActive()));
    add(ServerController::getInstance().getIp().set("IP", ServerController::getInstance().getIp()));
//    add(ofParameter<string>().set("Port", printf("%i",ServerController::getInstance().getPort().get())));
    add(ObjectController::getInstance().getMaxLines().set("Max. lines", ObjectController::getInstance().getMaxLines(), 0, 1));
    add(ObjectController::getInstance().getPulseTime().set("Pulse time", ObjectController::getInstance().getPulseTime(), 0, 1));
    add(ObjectController::getInstance().getDrawingSpeed().set("Speed", ObjectController::getInstance().getDrawingSpeed(), 0, 1));
    add(ObjectController::getInstance().getDrawingRange().set("Range", ObjectController::getInstance().getDrawingRange(), 0, 1));
    add(ObjectController::getInstance().getFadeoutTimeIdle().set("Fadeout idle", ObjectController::getInstance().getFadeoutTimeIdle(), 0, 1));
    add(ObjectController::getInstance().getFadeoutTimeGone().set("Fadeout gone", ObjectController::getInstance().getFadeoutTimeGone(), 0, 1));
    add(timeout_gone.set("gone timeout: ", timeout_gone));
    add(timeout_idle.set("idle timeout: ", timeout_idle));

    participants.setup("Connected drawers");
    participants.setBorderColor(ofColor::black);
    participants.setHeaderBackgroundColor(ofColor::fromHex(0xA09000));

}

ServerTab::~ServerTab() {
    save_btn.removeListener(&Traces::get(), &Traces::saveServer);
}

void ServerTab::update() {
    if(control_rect.position != this->getPosition()) {
        control_rect.position = this->getPosition();
        control_rect.setWidth(this->getWidth());
        control_rect.setHeight(this->getHeight());

        ofRectangle r_widget;
        int margin = 10;
        r_widget.x = this->getPosition().x+this->getWidth()+margin;
        r_widget.y = this->getPosition().y;
        r_widget.width = 420;
        r_widget.height = this->getHeight();

        participants.setShape(r_widget);
    }

    //updating idle and gone timeout strings
    int sec = ObjectController::getInstance().getFadeoutTimeGone()*ObjectController::getInstance().getMaxFadeoutTime();
    int seconds=sec%60;
    int minutes=(sec/(60))%60;
    int hours=(sec/(60*60))%24;
    stringstream timestr1;
    timestr1 << setfill('0') << setw(2) << hours;
    timestr1 << ":";
    timestr1 << setfill('0') << setw(2) << minutes;
    timestr1 << ":";
    timestr1 << setfill('0') << setw(2) << seconds;
    timeout_gone = timestr1.str();
    sec = ObjectController::getInstance().getFadeoutTimeIdle()*ObjectController::getInstance().getMaxFadeoutTime();
    seconds=sec%60;
    minutes=(sec/(60))%60;
    hours=(sec/(60*60))%24;
    stringstream timestr2;
    timestr2 << setfill('0') << setw(2) << hours;
    timestr2 << ":";
    timestr2 << setfill('0') << setw(2) << minutes;
    timestr2 << ":";
    timestr2 << setfill('0') << setw(2) << seconds;
    timeout_idle = timestr2.str();

}

void ServerTab::draw(ofPoint p) {

    if(visible) {

        CustomTab::draw(p);
        participants.draw(p);
        ofPushMatrix();
        drawServerStatus(participants.getShape());
        ofPopMatrix();

    }

}

void ServerTab::drawServerStatus(ofRectangle shape) {

    ofEnableAlphaBlending();
    ofSetLineWidth(1);
    ofFill();

    int status_quad = 20;
    int margin = 10;

    ofRectangle r_widget = shape;
    r_widget.y = shape.y+30;

    ofRectangle r_connected(r_widget);
    r_connected.height = status_quad+margin;

    ofRectangle r_clients(r_widget);
    r_clients.y += r_connected.height+margin;
    r_clients.height = r_widget.height-r_connected.height;

    ofRectangle r_client(0,0,0,0);
    r_client.width += r_clients.width;
    r_client.height = status_quad+margin;

    //int max_clients = r_clients.height/r_client.height-1;

    ofPushMatrix();
    ofTranslate(r_connected.x,r_connected.y);

        stringstream con_msg;
        //DRAWING CONNECTED STATUS
        if(ServerController::getInstance().isConnected()) {
            if(ServerController::getInstance().getActive()) {
                con_msg << "CONNECTED. ";
                ofSetColor(0,255,0);
            }
            else {
                con_msg << "CONNECTED BUT INACTIVE. ";
                ofSetColor(255,255,0);
            }
        }
        else {
            ofSetColor(255,0,0);
            con_msg << "DISCONNECTED. ";
        }
        ofDrawRectangle(margin,margin-status_quad/2,status_quad, status_quad);
        stringstream obj_msg;
        obj_msg << con_msg.str();
        obj_msg << "Drawing Objects: ";
        obj_msg << ObjectController::getInstance().getClients().size() << ". ";
        obj_msg << "Lines max.: " << ObjectController::getInstance().getMaxLinecount();
        ofSetColor(255);
        TTF.drawString(obj_msg.str(),status_quad+margin*2,margin*3/2);

    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(r_clients.x,r_clients.y);

        //DRAWING CLIENTS;
        // @TODO SORT BY MOST ACTIVE AND MOST INACTIVE
        map<string,DrawingObject_ptr>cs = ObjectController::getInstance().getClients();
        map<string,DrawingObject_ptr>::iterator iter;
        DrawingObject_ptr c;

        if(cs.size()>0) {
            vector<string> client_names;
            client_names.clear();
            int i = 0;
            for( iter = cs.begin(); iter != cs.end(); iter++ ) {
                i++;
                c = iter->second;
                if(c) {

                    client_names.push_back(c->getId());
                    ofSetColor(c->getColor());
                    ofDrawRectangle(margin,i*r_client.height-status_quad/2,status_quad, status_quad);
                    ofSetLineWidth(5);
                    ofSetColor(255,100,100,(1-c->getPulseVal())*255);
                    ofNoFill();
                    ofDrawRectangle(margin,i*r_client.height-status_quad/2,status_quad, status_quad);
                    ofFill();
                    ofSetLineWidth(1);
                    if(c->getConnections().size()>0)
                        ofSetColor(255);
                    else
                        ofSetColor(100);
                    stringstream c_status;
                    c_status << "ID: " << c->getId();
                    c_status << " lines: " << c->getConnections().size();
                    string resttime = c->getRestTimeAsString();
                    if(resttime != "") {
                        if(c->isGone()) {
                            c_status << " gone: ";
                        }
                        else {
                            c_status << " inactive: ";
                        }
                        c_status << resttime;
                    }
                    TTF.drawString(c_status.str(), status_quad+margin*2, i*r_client.height+margin/2);
                }
            }
        }

    ofPopMatrix();
}
