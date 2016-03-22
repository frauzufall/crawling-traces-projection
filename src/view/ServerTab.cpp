#include "ServerTab.h"

using namespace guardacaso;

ServerTab::ServerTab(const string &name, const ofJson &config):CustomTab(name, config) {
}

void ServerTab::setup(Traces *tc){

	traces_controller = tc;

	setName("Server");
	setConfig(group_config);

	TTF.load("mono", 10);

	settings = addPanel("settings", group_config);
	settings->setShowHeader(false);

	save.addListener(traces_controller, &Traces::saveServer);
	settings->add(save.set("SAVE"), toggle_config);

	settings->add(traces_controller->getServerController()->getActive(), toggle_config);
	settings->add(traces_controller->getServerController()->getIp(), label_config);
	settings->add<ofxIntLabel>(traces_controller->getServerController()->getPort(), label_config);
	settings->add(traces_controller->getObjectController()->getMaxLines(), slider_config);
	settings->add(traces_controller->getObjectController()->getPulseTime(), slider_config);
	settings->add(traces_controller->getObjectController()->getDrawingSpeed(), slider_config);
	settings->add(traces_controller->getObjectController()->getDrawingRangeMax(), slider_config);
	settings->add(traces_controller->getObjectController()->getDrawingRangeMin(), slider_config);
	settings->add(traces_controller->getObjectController()->getConnectToItself(), toggle_config);
	settings->add(traces_controller->getObjectController()->getConnectToOthers(), toggle_config);
	settings->add(traces_controller->getObjectController()->getMaxConnections(), slider_config);
	settings->add(traces_controller->getObjectController()->getFadeoutTimeIdle(), slider_config);
	settings->add(traces_controller->getObjectController()->getFadeoutTimeGone(), slider_config);
	settings->add(timeout_gone.set("gone timeout", timeout_gone), label_config);
	settings->add(timeout_idle.set("idle timeout", timeout_idle), label_config);

	traces_controller->getObjectController()->getFadeoutTimeIdle().addListener(this, &ServerTab::updateTimeoutIdle);
	traces_controller->getObjectController()->getFadeoutTimeGone().addListener(this, &ServerTab::updateTimeoutGone);

//    participants.setup("Connected drawers");
//    participants.setHeaderBackgroundColor(ofColor::fromHex(0xA09000));
//    participants.add(status_msg.setup("status","waiting"));
//    participants.addSpacer(10);

//    traces_controller->getServerController()->isConnected().addListener(this,&ServerTab::updateServerStatus);
//    traces_controller->getServerController()->getActive().addListener(this,&ServerTab::updateServerStatus);

//    add(participants);

}

ServerTab::~ServerTab() {
	save.removeListener(traces_controller, &Traces::saveServer);
	traces_controller->getObjectController()->getFadeoutTimeIdle().removeListener(this, &ServerTab::updateTimeoutIdle);
	traces_controller->getObjectController()->getFadeoutTimeGone().removeListener(this, &ServerTab::updateTimeoutGone);
//    traces_controller->getServerController()->isConnected().removeListener(this,&ServerTab::updateServerStatus);
//    traces_controller->getServerController()->getActive().removeListener(this,&ServerTab::updateServerStatus);
}

void ServerTab::updateTimeoutGone(float&){
	timeout_gone = secondsToTimestring(
				traces_controller->getObjectController()->getFadeoutTimeGone()
				*traces_controller->getObjectController()->getMaxFadeoutTime());
}

void ServerTab::updateTimeoutIdle(float&){
	timeout_idle = secondsToTimestring(
				traces_controller->getObjectController()->getFadeoutTimeIdle()
				*traces_controller->getObjectController()->getMaxFadeoutTime());
}

void ServerTab::updateServerStatus(bool &){
//	if(traces_controller->getServerController()->isConnected()) {
//	   if(traces_controller->getServerController()->getActive()) {
//		   status_msg = "connected";
//		   status_msg.setBackgroundColor(ofColor(0,255,0));
//	   }
//	   else {
//		   status_msg = "connected but inactive";
//		   status_msg.setBackgroundColor(ofColor(255,255,0));
//	   }
//   }
//   else {
//	   status_msg.setBackgroundColor(ofColor(255,0,0));
//	   status_msg = "disconnected";
//   }
}

std::string ServerTab::secondsToTimestring(int sec){
	int seconds, minutes, hours;
	seconds=sec%60;
	minutes=(sec/(60))%60;
	hours=(sec/(60*60))%24;
	stringstream timestr2;
	timestr2 << setfill('0') << setw(2) << hours;
	timestr2 << ":";
	timestr2 << setfill('0') << setw(2) << minutes;
	timestr2 << ":";
	timestr2 << setfill('0') << setw(2) << seconds;
	return timestr2.str();
}

void ServerTab::render(){

	CustomTab::render();

	participants.setPosition(settings->getPosition()+ofPoint(settings->getWidth()+margin, 0));
	participants.setWidth(this->getShape().getRight()-participants.getPosition().x - margin);
	participants.setHeight(this->getShape().getBottom()-participants.getPosition().y - margin);

	ofEnableAlphaBlending();
	ofSetLineWidth(1);
	ofFill();

	int status_quad = 20;

	ofRectangle r_widget = participants;

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
		if(traces_controller->getServerController()->isConnected()) {
			if(traces_controller->getServerController()->getActive()) {
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
		obj_msg << traces_controller->getObjectController()->getClients().size() << " drawing objects,";
		obj_msg << traces_controller->getObjectController()->getTotalClientLineCount() << " lines ";
		obj_msg << "(max. " << traces_controller->getObjectController()->getMaxLinecount() << ")";
		ofSetColor(255);
		TTF.drawString(obj_msg.str(),status_quad+margin*2,margin*3/2);

	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(r_clients.x,r_clients.y);

		//DRAWING CLIENTS;
		// @TODO SORT BY MOST ACTIVE AND MOST INACTIVE
		map<string,DrawingObject_ptr>cs = traces_controller->getObjectController()->getClients();
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
					ofSetLineWidth(3);
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
						if(c->getIsGone()) {
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
