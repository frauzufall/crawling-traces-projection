#include "ofMain.h"

#include "Visuals.h"
#include "CustomTab.h"
#include "ServerTab.h"
#include "MappingTab.h"
#include "PathsTab.h"

using namespace guardacaso;

namespace guardacaso {

    class ControlWindow {

        public:

            static ControlWindow&	getInstance();
            void setup();
            void update();
            void draw(ofPoint p);

            bool drawingMapping();

            bool isSetup();

            void keyPressed(int key);

            MappingTab& getMapping();

    protected:
        ControlWindow();
        ~ControlWindow() {
        }

    private:

            void saveAllSettings();

            CustomTab gui;
            CustomTab status;
            ServerTab server;
            MappingTab mapping;
            PathsTab paths;

            float header_height;

            int w,h,x,y;

            void setGui();

            bool is_setup;

            string font;

            bool gui_invert;

            ofxButton save_settings_btn, save_gui_btn, import_events_btn;

            ofParameter<string> status_temp;

            int system_temp_freq;
            int last_temp_update;

            string xml_gui;
            void reloadGui();
            void saveGui();
            void importGroup();

    };

}

