#pragma once

#include "ofMain.h"
#include "IDGenerator.h"

namespace guardacaso {

	class MappingQuad {
	public:
        MappingQuad() {
            setup("window");
        }

        MappingQuad(string nature) {
            stringstream id_str;
            id_str << IDGenerator::getInstance().next();
            id = id_str.str();
            setup(nature);
        }

        void setup(string nature) {
			
            this->nature = nature;
			
			src[0].x = 0;
			src[0].y = 0;
            src[1].x = 1;
			src[1].y = src[0].y;
			src[2].x = src[1].x;
            src[2].y = 1;
			src[3].x = src[0].x;
			src[3].y = src[2].y;
			
			dst[0].x = 0;
			dst[0].y = 0;
            dst[1].x = 1;
			dst[1].y = dst[0].y;
			dst[2].x = dst[1].x;
            dst[2].y = 1;
			dst[3].x = dst[0].x;
            dst[3].y = dst[2].y;

            polyline.clear();
            polyline.addVertex(dst[0]);
            polyline.addVertex(dst[1]);
            polyline.addVertex(dst[2]);
            polyline.addVertex(dst[3]);
            polyline.close();

            plane[0] = ofPoint(0,0);
            plane[1] = plane[0];
            plane[2] = plane[0];
            plane[3] = plane[0];

            matrix_src_dst.makeIdentityMatrix();
            matrix_dst_norm.makeIdentityMatrix();
            image.clear();
            img_src = "";

            newpos = true;
            showframe = true;
            editable = false;
            flag_delete = false;
			
        }
		
        string              id;
        ofPoint             src[4];
        ofPoint             dst[4];
        string              nature;
        bool                newpos;
        ofPoint             plane[4];
        ofMatrix4x4         matrix_src_dst;
        ofMatrix4x4         matrix_dst_norm;
        ofImage             image;
        ofParameter<string> img_src;
        ofParameter<bool>   showframe;
        ofPolyline          polyline;
        ofParameter<bool>   editable;
        ofParameter<bool>   flag_delete;

        ~MappingQuad(){}
		
	};
	
}
