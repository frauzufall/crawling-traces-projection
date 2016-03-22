#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ObjectController.h"
#include "ServerController.h"
#include "MappingController.h"

typedef std::shared_ptr<ofxXmlSettings> ofxXmlSettings_ptr;

namespace guardacaso {

    class Traces {
		
		public:
		
            Traces();
            ~Traces();

            void			setup(MappingController* mc);
            void			setup(MappingController* mc, string ip, int port);
            void			update();
            void			reloadServer(ofxXmlSettings_ptr xml);
            void			reloadServer(ofxXmlSettings_ptr xml, string ip, int port);
            void            saveServer();
            string          clientId();
            string          historyDir();
            ServerController* getServerController();
            ObjectController* getObjectController();

            void simulateGroup(MappingController* mc, string group_dir);
            void simulateGroups(MappingController* mc);
			
        private:

            void sendMapping(bool&);

            ObjectController object_controller;
            ServerController server_controller;
            MappingController* mapping_controller;

            void			setupServer(string ip, int port);
            void			setupServer();

            string          xml_server;
            string          client_id;
            ofParameter<string> history_dir;
			
	};
	
}
