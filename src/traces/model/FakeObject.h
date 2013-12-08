#pragma once

#include "LightObject.h"

class FakeObject : public LightObject {

    public:

        FakeObject(string future, string id);

        void update();

        ofPolyline getLine();
        ofPolyline getConnections();
        void clearLines();
        void setPos(ofPoint p, string timestamp);

        ofColor getModColor();

        bool hasFuture();
        void futureStep();

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
