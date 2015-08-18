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
        ofPolyline &getLine();
        ofPolyline getConnections();
        void setNewRandomColor();
        void clearLines();
        bool setPos(string timestamp, ofPoint p);
        void addIntersection(string timestamp, ofPoint p);

        ofParameter<ofColor>& getModColor();
        ofParameter<float>& getPulseStart();
        ofParameter<float>& getPulseVal();
        ofParameter<float>& getPulseDuration();

        //void setPulsing();

        void closeAndSave(string history_dir, float output_w, float output_h);

        ~DrawingObject();

    protected:

        ofParameter<bool> please_redraw;
        ofPolyline line;
        ofPolyline backup_line;

        ofPolyline history_line;
        vector<string> history_timestamp;

        ofPolyline history_net_line;
        vector<string> history_net_timestamp;

        ofPolyline future_line;
        vector<uint> future_timestamp;

        ofParameter<float> pulsestart;
        ofParameter<float> pulseval;
        ofParameter<float> pulseduration;

        void saveTimestamps(string path);
        void saveNetTimestamps(string path);
        void setFinishingPos();

};
