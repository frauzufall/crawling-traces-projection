#pragma once

#include "ofMain.h"
#include "Projector.h"

typedef std::shared_ptr<ofFbo> ofFbo_ptr;

namespace guardacaso {
	
    class MappingController {
		
		public:
            static MappingController&	getInstance();
            void						update();
            void						draw(int projector_id);
            void                        drawArea(Projector *p);
            void                        drawImages(Projector *p);
            void                        drawPaths(Projector *p);

            void						addProjector();
            Projector*                  getProjector(int id);

            ofPoint                     getPointInMappedArea(ofPoint last_p,ofPoint next_p);
            ofPoint                     intersectionPointPolyline(ofPoint last_p, ofPoint next_p, ofPolyline polyline);

            ofPoint                     getIntersection(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4);
            bool                        isLeft(ofPoint p1, ofPoint p2, ofPoint p_test);
            bool                        isOnLine(ofPoint p1, ofPoint p2, ofPoint p_test);

            ofParameter<bool>           getCalibrating();
            void                        setCalibrating(bool calibrate);

            ofParameter<float>          getCalBorder();
            ofParameter<int>            getCalGrey();
            void                        setCalBorder(float border);
            void                        setCalGrey(int grey);

            ofParameter<bool>           getUsingCam();
            void                        setUsingCam(bool in_use);

            void                        clear();

            ofParameter<bool>           is_cal;
            ofParameter<float>          cal_border;
            ofParameter<int>            cal_grey;
            ofParameter<bool>           use_cam;
				
		protected:
            MappingController();
            ~MappingController() {}
			
        private:

            vector<Projector>			projectors;
			GLfloat						matrix[16];
            ofPoint						plane[4];


            void                        drawCalibration(Projector *p);

	};
	
}

