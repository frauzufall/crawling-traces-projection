#pragma once

#include "ofMain.h"

namespace guardacaso {

    class Visuals {
		
		public:
		
            Visuals();
            ~Visuals();
			
            ofParameter<float>&			contentWidth();
            ofParameter<float>&			contentHeight();

            ofParameter<string> &xmlMappingPath();

            ofPoint inContent(ofPoint orig);
			
        private:
			
			/******* WINDOW ADJUSTMENTS *****/
            ofParameter<float>			content_w, content_h;
			
	};
	
}
