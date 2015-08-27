#include "ofMain.h"

#include "ofxGui.h"

#include "ServerTab.h"
#include "PathsTab.h"

#include "MappingController.h"
#include "PathsController.h"
#include "Traces.h"
#include "VideoRecorderController.h"

using namespace guardacaso;

namespace guardacaso {

    class ControlWindow : public ofxPanel {

        public:

            ControlWindow();
            ~ControlWindow();
            void setup(MappingController* mc, PathsController* pc, Traces* tc, VideoRecorderController *vrc);
            void update();
            void updatePosition(bool& left);

            bool isSetup();

            void keyPressed(int key);

    private:

            void saveAllSettings();

            MappingController* mapping_controller;
            PathsController* paths_controller;
            Traces* traces_controller;
            VideoRecorderController* video_recorder_controller;

            ofxTabbedPages gui;
            ofxGuiGroup header, status;
            ServerTab server;
            PathsTab paths;

            int w,h;

            bool is_setup;

            ofxMinimalButton save_settings_btn, import_events_btn;
            ofxLabel title;

            ofParameter<string> status_temp;
            ofParameter<bool> recording;

            int system_temp_freq;
            int last_temp_update;

            string xml_gui;
            void importGroup();

    };

}

