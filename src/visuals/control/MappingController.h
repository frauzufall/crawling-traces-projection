#pragma once

#include "ofMain.h"
#include "ofx2DMapping.h"

namespace guardacaso {
	
    class MappingController {
		
		public:
            MappingController();
            ~MappingController();

            void setup();
            void update();
            void draw(int projector_id = 0);
            void updatePosition(bool& left);
            ofParameter<bool>& controlLeft();
            ofPoint controlPoint();

            ofx2DMapping* getMapping();

            void importSvg(const string path);
			
        private:

            ofParameter<string> xml_mapping;
            ofParameter<bool> control_left;

            ofx2DMapping mapping;

	};
	
}

