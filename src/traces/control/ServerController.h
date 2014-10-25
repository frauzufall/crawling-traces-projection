
#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include <memory>

using namespace std;

namespace guardacaso {

#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

    class ServerController {

        public:
            static ServerController&     getInstance();
            void setup(string ip, int port, string name);
            void update();
            bool isConnected();
            bool send(string client_id, string action, string value);
            void sendMappingQuads();
            void askForColor(string client_id);
            ofParameter<bool> getActive();
            void setActive(bool _active);
            ofParameter<int> getPort();
            ofParameter<string> getIp();
            string getClientName();

        protected:
            ServerController();
            ~ServerController() {
//                udpConnection.Close();
//                tcpConnection.close();
            }

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

            ofParameter<int> port;
            ofParameter<string> ip;
            string client_name;

            bool connected;
            ofParameter<bool> active;

            int protocol;

            bool setup_done;

    };
}

