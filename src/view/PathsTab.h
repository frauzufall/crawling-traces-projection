#pragma once

#include "PathsController.h"
#include "CustomTab.h"

namespace guardacaso {

class PathsTab : public CustomTab {

	public:
		PathsTab(const std::string &name="Paths", const ofJson& config = ofJson());
		~PathsTab();

		void setup(PathsController* pc);
		void setGui(int &index);

	private:

		PathsController* paths_controller;

		float preview_h, preview_w;

		ofParameter<void> save;

		ofxGuiGroup *paths_list, *paths_options;

};

}
