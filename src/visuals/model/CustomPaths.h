#pragma once

#include "PathsData.h"
#include "ofx2DMappingProjector.h"

namespace guardacaso {

    class CustomPaths {
	
	public:
		
        CustomPaths(string title);
        ~CustomPaths(){}
		
		virtual void setup() = 0;
		
        virtual void update(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients) = 0;

        virtual void draw(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients) = 0;

        virtual void idle() = 0;
        virtual void resume() = 0;
		
		string getName();

        ofParameter<bool> &setupDone();
        bool isLoaded();
        void setLoaded();

        ofParameter<bool> &isActive();

        ofParameterGroup& getSettings();
		
    protected:
		
        ofParameterGroup settings;

		int delay;
        int lastupdate;
		
	private:
		
        string name;

        ofParameter<bool> setup_done;
        bool loaded;
        ofParameter<bool> active;
	
	};

}

