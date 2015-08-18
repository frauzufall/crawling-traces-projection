#pragma once

#include "ofMain.h"
#include "DrawingObject.h"
#include "FakeObject.h"
#include "ServerController.h"
#include "MappingController.h"

namespace guardacaso {

    typedef std::shared_ptr<DrawingObject> DrawingObject_ptr;
    typedef std::shared_ptr<FakeObject> FakeObject_ptr;

    class ObjectController {

        public:
            ObjectController();
            ~ObjectController() {
                pulsing_obj.reset();
                removeAllClients();
            }

            void setup(MappingController* mc);
            void update(ServerController *server_controller);

            ofParameter<string>& getHistoryDir();

            vector<DrawingObject_ptr> &getDrawingObjects();
            map<string, DrawingObject_ptr> &getClients();

            void removeObject(DrawingObject_ptr drawing_o);

            DrawingObject_ptr addDrawingObject(string id);

            void setObjectPos(DrawingObject_ptr obj, ofPoint p);
            void setFakeObjectPos(FakeObject_ptr obj, ofPoint p, string timestamp);

            ofParameter<float>& getDrawingSpeed();
            ofParameter<float>& getDrawingSpeedWheels();
            ofParameter<float>& getDrawingRangeMax();
            ofParameter<float>& getDrawingRangeMin();
            ofParameter<float>& getDrawingRangeWheels();
            int getMaxLinecount();
            ofParameter<float>& getPulseTime();
            float getMaxPulseTime();
            ofParameter<float>& getMaxLines();
            ofParameter<float>& getFadeoutTimeIdle();
            ofParameter<float>& getFadeoutTimeGone();
            ofParameter<float>& getMaxFadeoutTime();
            ofParameter<bool>& getConnectToItself();
            ofParameter<bool>& getConnectToOthers();
            ofParameter<int>& getMaxConnections();

            ofPoint getStartingPoint();
            ofPoint getCurrentPulsingPoint();

            int getTotalClientLineCount();
            DrawingObject_ptr getMostInactiveClient();
            DrawingObject_ptr getClient(string id);
            void fadeoutClient(string id);
            void deactivateClient(string id);
            void activateClient(string id);
            void removeClient(string id);
            void removeClient(map<string,DrawingObject_ptr>::iterator it);
            void removeAllClients();

            FakeObject_ptr addFakeObj(string path, string id);

            void processMsg(ctMessage& msg);

            ofEvent<ctMessage> sendingPosition;

        private:

            ofParameter<string> history_dir;

            MappingController* mapping_controller;

            vector<DrawingObject_ptr> draw_obj;
            map<string, DrawingObject_ptr> clients;

            ofParameter<float> client_speed;
            ofParameter<float> wheel_speed;
            ofParameter<float> client_range_max;
            ofParameter<float> client_range_min;
            ofParameter<float> wheel_range;
            ofParameter<float> max_lines_percent;
            ofParameter<int> max_lines;
            ofParameter<float> pulse_time_percent;
            ofParameter<float> max_pulse_time;
            ofParameter<float> fadeout_time_idle;
            ofParameter<float> fadeout_time_gone;
            ofParameter<float> max_fadeout_time;
            ofParameter<bool> connect_to_itself;
            ofParameter<bool> connect_to_others;
            ofParameter<int> max_connections;
            DrawingObject_ptr pulsing_obj;

            vector<FakeObject_ptr> fake_objs;
    };

}
