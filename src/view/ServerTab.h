#pragma once

#include "Traces.h"
#include "CustomTab.h"

namespace guardacaso {

class ServerTab : public CustomTab {

	public:
		ServerTab(const std::string &name="Server", const ofJson& config = ofJson());
		~ServerTab();

		void setup(Traces* tc);
		void render();

		void updateTimeoutGone(float&);
		void updateTimeoutIdle(float&);
		void updateServerStatus(bool&);

	private:

		Traces* traces_controller;

		std::string secondsToTimestring(int seconds);

		ofParameter<void> save;
		ofxPanel *settings;
		ofRectangle participants;
//		ofParameter<std::string> status_msg;

		ofTrueTypeFont TTF;

		ofParameter<string> timeout_idle;
		ofParameter<string> timeout_gone;

};

}
