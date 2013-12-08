#pragma once

#include "LightObject.h"

class DrawingObject : public LightObject {

    public:

        DrawingObject();

        DrawingObject(string future);

        void setup(string id);

        void update();

        void gotRedrawn();
        bool needsRedraw();
        ofMesh getMesh();
        ofPolyline getLine();
        ofPolyline getConnections();
        void setNewRandomColor();
        void clearLines();
        void setPos(ofPoint p);

        ofColor getModColor();
        float getPulseStart();

        void setPulsing();

        float getPulseVal();
        void setPulseVal(float val);

        ~DrawingObject();

    protected:

        bool please_redraw;
        ofPolyline line;
        ofPolyline backup_line;

        ofPolyline history_line;
        vector<string> history_timestamp;

        ofPolyline history_net_line;
        vector<string> history_net_timestamp;

        ofPolyline future_line;
        vector<uint> future_timestamp;

        float range;
        float speed;

        float pulsestart;
        float pulseval;

        void saveTimestamps(string path);
        void saveNetTimestamps(string path);
        void setFinishingPos();

};
