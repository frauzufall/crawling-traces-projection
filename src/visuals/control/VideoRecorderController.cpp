#include "VideoRecorderController.h"

using namespace guardacaso;
//--------------------------------------------------------------
VideoRecorderController::VideoRecorderController(){

    is_recording = false;

}

void VideoRecorderController::setup(ofPtr<ofFbo> fbo) {

    output_fbo = fbo;

    pixels.allocate(output_fbo->getWidth(), output_fbo->getHeight(),OF_IMAGE_COLOR);

    recorder.setVideoCodec("mpeg4");
    recorder.setVideoBitrate("10000k");

}

void VideoRecorderController::update() {

    if(is_recording) {
        output_fbo->readToPixels(pixels);
        recorder.addFrame(pixels);
    }
	
}

void VideoRecorderController::startRecording() {

    is_recording = !is_recording;
    if(is_recording && !recorder.isInitialized()) {

        stringstream filename;
        filename << "records/record-" << ofGetTimestampString() << ".mp4";

        recorder.setup(filename.str(), output_fbo->getWidth(), output_fbo->getHeight(), 30);
        recorder.start();

    }

}

void VideoRecorderController::stopRecording() {

    is_recording = false;
    recorder.close();

}

void VideoRecorderController::setRecording(bool &recording) {
    if(recording && !is_recording) {
        startRecording();
    }
    if(!recording && is_recording) {
        stopRecording();
    }
}
