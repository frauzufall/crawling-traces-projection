#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

namespace guardacaso {

    class VideoRecorderController {
		
		public:
            VideoRecorderController();
            ~VideoRecorderController() {
                recorder.close();
            }

            void						setup(ofPtr<ofFbo> fbo);
            void						update();
            void                        startRecording();
            void                        stopRecording();
            void                        setRecording(bool& recording);
			
        private:

            ofPtr<ofFbo> output_fbo;
            ofPixels pixels;
            ofxVideoRecorder recorder;
            ofParameter<bool> is_recording;
			

	};
	
}
