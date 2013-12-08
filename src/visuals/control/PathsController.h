#pragma once

#include "CustomPaths.h"

using namespace guardacaso;

typedef tr1::shared_ptr<CustomPaths> CustomPaths_ptr;

namespace guardacaso {

    class PathsController {
		
	public:
        static PathsController&	getInstance();

		void						update();
        void                        draw();
		
        void						addPaths(CustomPaths_ptr c);
        CustomPaths_ptr             getActivePath();
        CustomPaths_ptr             getPath(int index);
        bool                        activePathChanged();
        void						setActivePath(string paths_name, bool loadDataFromLastPaths);
        void						setActivePath(int paths_name, bool loadDataFromLastPaths);
        vector<string>              getPathsNames();
		
	protected:
        PathsController();
        ~PathsController() {}

		
    private:
		
        vector<CustomPaths_ptr>             paths;
        ofParameter<int>                    active_path;
        float                               w,h;
		
	};

}
