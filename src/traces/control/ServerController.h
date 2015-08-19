
#pragma once

#include "ofMain.h"
#include "ofx2DMappingProjector.h"
#include "ofxNetwork.h"
#include <memory>

namespace guardacaso {

    struct ctMessage{
        string client_id;
        string action;
        string value;
    };

    #define PROTOCOL_TCP 0
    #define PROTOCOL_UDP 1

    class ServerController {

        public:
            ServerController();
            ~ServerController() {
        //                udpConnection.Close();
        //                tcpConnection.close();
            }

            void setup(string ip, int port, string name);
            void update();
            ofParameter<bool>& isConnected();
            bool send(string client_id, string action, string value);
            void sendMappingQuads(ofx2DMappingProjector *projector);
            void askForColor(string client_id);
            ofParameter<bool> &getActive();
            void setActive(bool _active);
            ofParameter<int> &getPort();
            ofParameter<string> &getIp();
            ofParameter<string>& getClientName();
            void sendPosition(ctMessage& msg);

            ofEvent<ctMessage> messageReceived;
            ofEvent<bool> mappingRequested;

        private:

            void gotMessage(ofMessage msg);
            void reconnect();
            void reconnectTcp();
            void reconnectUdp();
            void setupUdp(string address, int port);
            string read();
            string readTcp();
            string readUdp();
            bool sendViaTcp(string client_id, string action, string value);
            bool sendViaUdp(string client_id, string action, string value);
            void processMsg(string client_id, string action, string value);

            ofxUDPManager udpConnection;
            ofxTCPClient tcpConnection;

            float reconnect_time;
            float last_reconnect;

            int mapping_sent;
            int mapping_time;

            ofParameter<int> port;
            ofParameter<string> ip;
            ofParameter<string> client_name;

            ofParameter<bool> connected;
            ofParameter<bool> active;

            int protocol;

            bool setup_done;

    };
}

