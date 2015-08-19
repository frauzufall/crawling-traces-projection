#pragma once

#include "CustomPaths.h"
#include "ofx2DMappingProjector.h"

using namespace guardacaso;

typedef std::shared_ptr<CustomPaths> CustomPaths_ptr;

namespace guardacaso {

    class PathsController {
		
    public:

        PathsController();
        ~PathsController();

        void setupPaths();
        void reloadPaths(ofPtr<ofxXmlSettings> xml);
        void savePaths();

        void setup();
        void update(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients);
        void draw(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients);
		
        void addPaths(CustomPaths_ptr c);
        CustomPaths_ptr getActivePath();
        CustomPaths_ptr getPath(int index);
        void activePathChanged(int &index);
        void setActivePath(string paths_name);
        void setActivePath(int index);
        vector<string> getPathsNames();

		
    private:

        ofParameter<string>         xml_paths;
		
        vector<CustomPaths_ptr>             paths;
        ofParameter<int>                    active_path;
        float                               w,h;
		
	};

}
