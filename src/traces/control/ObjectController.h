#pragma once

#include "ofMain.h"
#include "DrawingObject.h"
#include "FakeObject.h"

typedef std::shared_ptr<DrawingObject> DrawingObject_ptr;
typedef std::shared_ptr<FakeObject> FakeObject_ptr;

class ObjectController {

    public:
        static ObjectController& getInstance();

        void setup();
        void update();

        vector<DrawingObject_ptr> getDrawingObjects();
        map<string, DrawingObject_ptr> getClients();

        void removeObject(DrawingObject_ptr drawing_o);

        void setDrawingSpeed(float speed);
        ofParameter<float> getDrawingSpeed();
        void setDrawingSpeedWheels(float speed);
        ofParameter<float> getDrawingSpeedWheels();
        void setDrawingRange(float range);
        ofParameter<float> getDrawingRange();
        void setDrawingRangeWheels(float range);
        ofParameter<float> getDrawingRangeWheels();
        int getMaxLinecount();
        ofParameter<float> getPulseTime();
        float getMaxPulseTime();
        void setPulseTime(float time);
        ofParameter<float> getMaxLines();
        void setMaxLines(float percentage);
        ofParameter<float> getFadeoutTimeIdle();
        void setFadeoutTimeIdle(float time);
        ofParameter<float> getFadeoutTimeGone();
        void setFadeoutTimeGone(float time);
        ofParameter<float> getMaxFadeoutTime();

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

    protected:
        ObjectController();
        ~ObjectController() {
            pulsing_obj.reset();
            removeAllClients();
        }

    private:

        vector<DrawingObject_ptr> draw_obj;
        map<string, DrawingObject_ptr> clients;

        ofParameter<float> client_speed;
        ofParameter<float> wheel_speed;
        ofParameter<float> client_range;
        ofParameter<float> wheel_range;
        ofParameter<float> max_lines_percent;
        int max_lines;
        ofParameter<float> pulse_time_percent;
        ofParameter<float> max_pulse_time;
        ofParameter<float> fadeout_time_idle;
        ofParameter<float> fadeout_time_gone;
        float max_fadeout_time;
        DrawingObject_ptr pulsing_obj;

        vector<FakeObject_ptr> fake_objs;
};

