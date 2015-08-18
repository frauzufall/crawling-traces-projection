#pragma once

#include "PathsData.h"

namespace guardacaso {

    class CustomPaths {
	
	public:
		
        CustomPaths(string title);
        ~CustomPaths(){}
		
		virtual void setup() = 0;
		
        virtual void update(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients) = 0;

        virtual void draw(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients) = 0;

        virtual void idle() = 0;
        virtual void resume() = 0;
		
		string getName();

        bool isSetupDone();
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

        bool setup_done;
        bool loaded;
        ofParameter<bool> active;
	
	};

}

