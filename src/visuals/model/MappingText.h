#pragma once

#include "Settings.h"

namespace guardacaso {

    class MappingText {
	public:
        MappingText() {
            setup("xxx");
        }

        MappingText(string text) {
            setup(text);
        }

        void setup(string text) {
			
            this->content = text;
			
			dst[0].x = 0;
			dst[0].y = 0;
			dst[1].x = Settings::get().outputWidth();
			dst[1].y = dst[0].y;
			dst[2].x = dst[1].x;
			dst[2].y = Settings::get().outputHeight();
			dst[3].x = dst[0].x;
            dst[3].y = dst[2].y;
			
		}

        ofPoint     dst[4];
        string      content;

        ~MappingText(){}
		
	};
	
}
