#include "ServerController.h"
#include <exception>

using namespace guardacaso;

ServerController::ServerController() {

    setup_done = false;
    active = false;
    connected = false;
    ofSetVerticalSync(true);
    protocol = PROTOCOL_TCP;
    reconnect_time = 5;
    ip="";
    port = 8080;
    client_name="";
    mapping_sent = 0;
    mapping_time = 100;

}

void ServerController::setup(string address, int port, string name) {

    this->ip = address;
    this->port = port;
    client_name = name;
    cout << "SERVERCONTROLLER: name of this instance of WALLS: " << name << endl;

    setup_done = true;

    if(active)
        reconnect();

}

void ServerController::update() {

    if(setup_done && active) {

        float current_time = ofGetElapsedTimef();

        if(current_time - last_reconnect > reconnect_time) {
            if (!isConnected()){
                reconnect();
            }
            else {
                last_reconnect = ofGetElapsedTimef();
                if(!send(client_name, "test", "of")) {
                    connected = false;
                    reconnect();
                }
            }
        }
        if (isConnected()) {

            string message = read();
            while(message != "") {
                if(message!=""){
                    vector<string> msg_parts = ofSplitString(message,":");
                    if(msg_parts.size()==3) {
                        processMsg(msg_parts[0],msg_parts[1],msg_parts[2]);
                    }
                    //cout << "SERVERCONTROLLER LOG: msg: " << message << endl;
                }
                message = read();
            }
        }
    }

}

void ServerController::askForColor(string client_id) {
    if(connected && setup_done) {
        cout << "SERVERCONTROLLER: asking for color of client " << client_id << "." << endl;
        send(client_name,"getcolor",client_id);
    }
}


void ServerController::processMsg(string client_id, string action, string value) {

    if(setup_done) {
        if(action == "getmapping") {
            bool b = true;
            ofNotifyEvent(mappingRequested,b,this);
        }else{
            ctMessage msg;
            msg.client_id = client_id;
            msg.action = action;
            msg.value = value;
            ofNotifyEvent(messageReceived, msg, this);
        }

    }

}

void ServerController::sendMappingQuads(ofx2DMappingProjector* projector) {

    //ensure that mapping gets not sent too often (breaks connection of control webpage)
    int currentTime = ofGetElapsedTimeMillis();
    if(currentTime-mapping_sent > mapping_time) {
        mapping_sent = currentTime;

        send(client_name, "clearmappingforms", "");
        stringstream msg0;
        msg0 << projector->outputWidth() << "|" << projector->outputHeight();
        send(client_name, "mappingsize", msg0.str());
        for(int i = projector->shapeCount()-1; i >= 0; i--) {
            ofPtr<ofx2DMappingObject> mq = projector->getMappingObject(i);
            ofPolyline line = projector->outlinesRaw()->at(i);
            stringstream msg;
            msg << mq->id << ";" << mq->nature << ";";
            for(uint j = 0; j < line.getVertices().size(); j++) {
                if(j > 0) {
                    msg << ",";
                }
                ofPoint p = line.getVertices().at(j);
                if(projector->getUsingCam()) {
                    p = projector->inCameraView(p);
                }
                msg << p.x << "|" << p.y;
            }
            send(client_name, "newmappingform", msg.str());
        }
    }
}

string ServerController::read() {
    if(setup_done) {
        switch(protocol) {
        case PROTOCOL_UDP:
            return readUdp();
        case PROTOCOL_TCP:
            return readTcp();
        default:return "";
        }
    }
    return "";
}

string ServerController::readUdp() {
    char udpMessage[100000];
    udpConnection.Receive(udpMessage,100000);
    return (string)udpMessage;
}

string ServerController::readTcp() {
    if(!tcpConnection.isConnected()) {
        reconnect();
    }
    return tcpConnection.receive();
}

bool ServerController::send(string client_id, string action, string value) {
    if(setup_done) {
        switch(protocol) {
        case PROTOCOL_UDP:
            return sendViaUdp(client_id, action, value);
        case PROTOCOL_TCP:
            return sendViaTcp(client_id, action, value);
        default:return false;
        }
    }
    return false;
}

bool ServerController::sendViaTcp(string client_id, string action, string value) {
    string message = client_id + ":" + action + ":" + value;
    try {
        connected = tcpConnection.send(message);
    }
    catch (exception& e) {
        cout << "SERVERCONTROLLER: " << e.what() << endl;
    }
    return connected;
}

bool ServerController::sendViaUdp(string client_id, string action, string value) {
    string message = client_id + ":" + action + ":" + value;
    connected = udpConnection.Send(message.c_str(),message.length());
    return connected;
}

void ServerController::reconnectUdp() {
    string message = client_name + ":new:of";
    int sent = send(client_name, "new", "of");
    if(!sent) {
        cout << "SERVERCONTROLLER: no udp connection" << endl;
        connected = false;
        setupUdp(ip, port);
    }
    else
        connected = true;
}

void ServerController::setupUdp(string address, int port) {
    udpConnection.Create();
    udpConnection.Connect(address.c_str(),port);
    cout << "SERVERCONTROLLER LOG: connecting to " << ip << " on port " << port << endl;
    udpConnection.SetNonBlocking(true);
}

void ServerController::reconnectTcp() {
    connected = tcpConnection.setup(ip,port);
    tcpConnection.setMessageDelimiter("\n");
    if(connected) {
        if(send(client_name, "new", "of")) {
            cout << "SERVERCONTROLLER: established tcp connection and successfully sent message" << endl;
            connected = true;
        }
        else {
            cout << "SERVERCONTROLLER: established tcp connection, but could not send message" << endl;
        }
    }
    else {
        connected = false;
        cout << "SERVERCONTROLLER: no tcp connection" << endl;
    }
}

void ServerController::reconnect() {
    if(setup_done) {
        switch(protocol) {
        case PROTOCOL_UDP:
            reconnectUdp();
            break;
        case PROTOCOL_TCP:
            reconnectTcp();
            break;
        default:break;
        }
        last_reconnect = ofGetElapsedTimef();
    }
}

ofParameter<bool> &ServerController::isConnected() {
    return connected;
}

ofParameter<bool> ServerController::getActive() {
    return active;
}

void ServerController::setActive(bool _active) {
    if(_active && !isConnected() && setup_done)
        reconnect();
    active = _active;
}

ofParameter<int> &ServerController::getPort() {
    return port;
}

ofParameter<string> &ServerController::getIp() {
    return ip;
}

string ServerController::getClientName() {
    return client_name;
}

void ServerController::sendPosition(ctMessage &msg){
    send(getClientName(), msg.action, msg.value);
}
