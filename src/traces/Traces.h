#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

typedef std::shared_ptr<ofxXmlSettings> ofxXmlSettings_ptr;

namespace guardacaso {

    class Traces {
		
		public:
		
            static Traces&	get();
            void			setup();
            void			setup(string ip, int port);

            void			update();
            void			reloadServer(ofxXmlSettings_ptr xml);
            void			reloadServer(ofxXmlSettings_ptr xml, string ip, int port);
            void            saveServer();
            string          clientId();
            string          historyDir();

            void simulateGroup(string group_dir);
            void simulateGroups();

		protected:

            Traces();
            ~Traces();
			
        private:

            void			setupServer(string ip, int port);
            void			setupServer();

            string          xml_server;
            string          client_id;
            string          history_dir;
			
	};
	
}
