#pragma once

#include "ofxGui.h"

namespace guardacaso {

	class CustomTab : public ofxPanel {

	public:

		CustomTab(const string &name, const ofJson &config):ofxPanel(name, config){

			margin = 10;

//			group_config["spacing"] = 7;
//			group_config["header-background-color"] = ofColor::fromHex(0x368082);
//			group_config["background-color"] = ofColor(0,0,0,0);
//			group_config["fill-color"] = ofColor(255,150);
//			group_config["border-color"] = ofColor(0,0,0,0);

//			slider_config["background-color"] = ofColor(255, 66);
//			slider_config["fill-color"] = ofColor(255,150);
//			slider_config["border-color"] = ofColor(0,0,0,0);
//			slider_config["precision"] = 2;

//			toggle_config["background-color"] = ofColor(0,0,0,0);
//			toggle_config["fill-color"] = ofColor(255,150);
//			toggle_config["border-color"] = ofColor(255,66);

//			toggle_auto_width_config = toggle_config;
//			toggle_auto_width_config["width"] = 0;

//			label_config["background-color"] = ofColor(0,0,0,0);
//			label_config["text-color"] = ofColor(255);
//			label_config["border-color"] = ofColor(0,0,0,0);

		}

		~CustomTab(){}

		using ofxGuiGroup::setup;

		int margin;

		ofJson slider_config;
		ofJson toggle_config;
		ofJson toggle_auto_width_config;
		ofJson label_config;
		ofJson group_config;


	};

}
