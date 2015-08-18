#pragma once

#include "LightObject.h"

struct futureStepData{
    string timestamp;
    ofPoint pos;
};

class FakeObject : public LightObject {

    public:

        FakeObject(string future, string id);

        void update(float range_max, float speed);

        ofPolyline getLine();
        ofPolyline getConnections();
        void clearLines();
        bool setPos(string timestamp, ofPoint p);
        void addIntersection(string timestamp, ofPoint p);

        ofColor getModColor();

        bool hasFuture();
        futureStepData getFutureStep();
        void removeFutureStep();

        void closeAndSave(string history_dir, float output_w, float output_h);

        ~FakeObject();

    protected:

        ofPolyline line;
        ofPolyline backup_line;

        ofPolyline history_line;
        vector<string> history_timestamp;

        ofPolyline history_net_line;
        vector<string> history_net_timestamp;

        ofPolyline future_line;
        vector<string> future_timestamp;

        float range;
        float speed;

        void saveTimestamps(string path);
        void saveNetTimestamps(string path);

        void addHistoryNetLine(ofPoint p1, ofPoint p2, string timestamp);

};
