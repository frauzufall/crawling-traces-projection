#include "FormMapping.h"
#include "MappingController.h"
#include "ServerController.h"

using namespace guardacaso;

FormMapping::FormMapping(): CustomTab() {

    setup("MAPPING FORMS");

    img_col = ofColor::fromHex(0x2da1e3);
    pic_col = ofColor(200,0,200);
    win_col = ofColor(150,150,150);
    cam_col = ofColor(255,100,100);

    quads_dst.clear();

    control_rect = this->getShape();

}

void FormMapping::setMappingRects() {

    int margin = 10;
    int header = 20;

    mapping_rect_dst.x = this->getPosition().x+margin;
    mapping_rect_dst.y = this->getPosition().y+margin+header;

    mapping_rect_dst.height = this->getHeight()-margin-header;
    float output_ratio = (float)Visuals::get().outputWidth()/(float)Visuals::get().outputHeight();
    float output_ratio_inv = (float)Visuals::get().outputHeight()/(float)Visuals::get().outputWidth();
    mapping_rect_dst.width = mapping_rect_dst.height*output_ratio;
    if(mapping_rect_dst.width > this->getWidth()-2*margin) {
        mapping_rect_dst.width = this->getWidth()-2*margin;
        mapping_rect_dst.height = mapping_rect_dst.width*output_ratio_inv;
    }
    this->setSize(mapping_rect_dst.getWidth()+2*margin, mapping_rect_dst.getHeight()+header+2*margin);

}

void FormMapping::updateForms(int projector_id) {

    quads_dst.clear();
    int existing_quads = 0;
    Projector* p = MappingController::getInstance().getProjector(projector_id);

    for (uint i = 0; i < p->quadCount(); i++){

        ofPolyline line = p->getQuad(i)->polyline;

        quads_dst.push_back(vector<draggableVertex>());

        for(uint j = 0; j < line.size(); j++) {
            quads_dst[existing_quads].push_back(draggableVertex());
            quads_dst[existing_quads][j].quadIndex = i;
            quads_dst[existing_quads][j].x = line[j].x*mapping_rect_dst.width+mapping_rect_dst.x;
            quads_dst[existing_quads][j].y = line[j].y*mapping_rect_dst.height+mapping_rect_dst.y;
            quads_dst[existing_quads][j].bOver 			= false;
            quads_dst[existing_quads][j].bBeingDragged 	= false;
            quads_dst[existing_quads][j].radius = 6;
        }
        existing_quads++;
    }

    start_point.x = p->getStartPoint().x*mapping_rect_dst.width+mapping_rect_dst.x;
    start_point.y = p->getStartPoint().y*mapping_rect_dst.height+mapping_rect_dst.y;
    start_point.bOver = false;
    start_point.bBeingDragged = false;
    start_point.radius = 13;

    camera.clear();
    for(uint i = 0; i < 4; i++) {
        draggableVertex v;
        v.x = p->getCamera()[i].x*mapping_rect_dst.width+mapping_rect_dst.x;
        v.y = p->getCamera()[i].y*mapping_rect_dst.height+mapping_rect_dst.y;
        v.bOver = false;
        v.bBeingDragged = false;
        v.radius = 8;
        camera.push_back(v);
    }

}

void FormMapping::update() {

    use_cam = MappingController::getInstance().getUsingCam();

    if(control_rect.position != this->getPosition()) {
        control_rect.position = this->getPosition();
        control_rect.setWidth(this->getWidth());
        control_rect.setHeight(this->getHeight());

        setMappingRects();
        updateForms(0);
    }

}

void FormMapping::draw(ofPoint pos, int projector_id) {

    CustomTab::draw(pos);

    ofEnableAlphaBlending();

    ofSetLineWidth(1);

    ofPushMatrix();

    ofTranslate(mapping_rect_dst.position);
    ofScale(mapping_rect_dst.width/(float)Visuals::get().outputWidth(), mapping_rect_dst.width/(float)Visuals::get().outputWidth());

    ofFill();
    ofSetColor(33);
    ofDrawRectangle(0,0,Visuals::get().outputWidth(),Visuals::get().outputHeight());

    ofNoFill();
    ofSetColor(80,130,150);

    for (int i = quads_dst.size()-1; i >= 0; i--){

        MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex);

        ofBeginShape();
        ofPoint vertex;
        for(uint j = 0; j < 4; j++) {
            vertex.x = mq->dst[j].x*(float)Visuals::get().outputWidth();
            vertex.y = mq->dst[j].y*(float)Visuals::get().outputHeight();
            ofVertex(vertex);
        }
        ofEndShape(true);

    }

    ofPopMatrix();

    //DRAW LINES

    for (int i = quads_dst.size()-1; i >= 0; i--){
        string quadtype = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex)->nature;
        ofColor quad_col;
//        if(quad_list != 0) {
            if(quadtype == "painting") {
                quad_col = img_col;
            }
            if(quadtype == "picture") {
                quad_col = pic_col;
            }
            if(quadtype == "window") {
                quad_col = win_col;
            }
            else {
                ofSetColor(0);
            }
//        }
//        else {
//            ofSetColor(0);
//        }
        for(int j = 0; j < 2; j++) {
            if(j == 0) {
                ofFill();
                ofSetColor(quad_col,40);
                if(quadtype == "window")
                    ofSetColor(ofColor(0),80);
            }
            else {
                ofNoFill();
                ofSetColor(quad_col,255);
            }
            ofBeginShape();
            for(uint k = 0; k < quads_dst[i].size(); k++) {
                ofVertex(quads_dst[i][k].x, quads_dst[i][k].y);
            }
            ofEndShape(true);
        }
    }

    if(use_cam) {
        //draw camera field
        ofNoFill();
        ofSetColor(cam_col,255);
        ofBeginShape();
        for(uint i = 0; i < camera.size(); i++) {
            ofVertex(camera[i].x, camera[i].y);
        }
        ofEndShape(true);
    }

    //DRAW CIRCLES

    ofSetColor(255,255,255,200);
    for (int i = quads_dst.size()-1; i >= 0; i--){

        for(uint j = 0; j < quads_dst[i].size(); j++) {
            bool editable = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex)->editable;
            if(editable) {
                if (quads_dst[i][j].bOver) ofFill();
                else ofNoFill();
                ofDrawCircle(quads_dst[i][j].x, quads_dst[i][j].y,6);
            }

        }
    }
    ofSetColor(0,255,255,200);
    if(start_point.bOver)
        ofFill();
    else ofNoFill();
    ofDrawCircle(start_point.x, start_point.y,13);

    if(use_cam) {
        ofSetColor(cam_col,200);
        for(uint i = 0; i < camera.size(); i++) {
            draggableVertex v = camera.at(i);
            if(v.bOver)
                ofFill();
            else ofNoFill();
            ofDrawCircle(v.x, v.y,8);
        }
    }

}

ofColor FormMapping::getColorOf(string name) {
    if(name == "painting") return img_col;
    else if(name == "picture") return pic_col;
    else if(name == "window") return win_col;
    else return ofColor(0);
}

bool FormMapping::mouseMoved(ofMouseEventArgs& args) {

    ofPoint mouse(args.x,args.y);
    for (uint i = 0; i < quads_dst.size(); i++){
        for (uint j = 0; j < quads_dst[i].size(); j++){
            if (mouse.distance(quads_dst[i][j].asPoint()) < quads_dst[i][j].radius){
                quads_dst[i][j].bOver = true;
            } else {
                quads_dst[i][j].bOver = false;
            }
        }
    }
    if(mouse.distance(start_point.asPoint()) < start_point.radius)
        start_point.bOver = true;
    else
        start_point.bOver = false;

    if(use_cam) {
        for (uint i = 0; i < camera.size(); i++){
            if (mouse.distance(camera[i].asPoint()) < camera[i].radius){
                camera[i].bOver = true;
            } else {
                camera[i].bOver = false;
            }
        }
    }

    return CustomTab::mouseMoved(args);
}


bool FormMapping::mouseDragged(ofMouseEventArgs &args) {

    ofPoint mouse(args.x,args.y);

    for (uint i = 0; i < quads_dst.size(); i++){

        MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex);
        ofPolyline& line = mq->polyline;

        for (uint j = 0; j < quads_dst[i].size(); j++){

            if (quads_dst[i][j].bBeingDragged == true){
                if(mouse.x < mapping_rect_dst.x+mapping_rect_dst.width) {
                    if(mouse.x > mapping_rect_dst.x)
                        quads_dst[i][j].x = mouse.x;
                    else
                        quads_dst[i][j].x = mapping_rect_dst.x;
                }
                else {
                    quads_dst[i][j].x = mapping_rect_dst.x+mapping_rect_dst.width;
                }
                if(mouse.y < mapping_rect_dst.y+mapping_rect_dst.height) {
                    if(mouse.y > mapping_rect_dst.y)
                        quads_dst[i][j].y = mouse.y;
                    else
                        quads_dst[i][j].y = mapping_rect_dst.y;
                }
                else {
                    quads_dst[i][j].y = mapping_rect_dst.y+mapping_rect_dst.height;
                }

                if(quads_dst[i].size() == 4) {

                    mq->dst[j].x = (quads_dst[i][j].x-mapping_rect_dst.x)/mapping_rect_dst.width;
                    mq->dst[j].y = (quads_dst[i][j].y-mapping_rect_dst.y)/mapping_rect_dst.height;

                    line[j].x = mq->dst[j].x;
                    line[j].y = mq->dst[j].y;

                }
                else {

                    line[j].x = (quads_dst[i][j].x-mapping_rect_dst.x)/mapping_rect_dst.width;
                    line[j].y = (quads_dst[i][j].y-mapping_rect_dst.y)/mapping_rect_dst.height;

                    ofRectangle bounding = line.getBoundingBox();

                    mq->dst[0].x = bounding.x;
                    mq->dst[0].y = bounding.y;
                    mq->dst[1].x = bounding.x+bounding.width;
                    mq->dst[1].y = bounding.y;
                    mq->dst[2].x = bounding.x+bounding.width;
                    mq->dst[2].y = bounding.y+bounding.height;
                    mq->dst[3].x = bounding.x;
                    mq->dst[3].y = bounding.y+bounding.height;

                }

                mq->newpos = true;
                //Visuals::get().reloadLinesFromRaw();
            }
        }
    }
    if(start_point.bBeingDragged) {
        if(mouse.x < mapping_rect_dst.x+mapping_rect_dst.width) {
            if(mouse.x > mapping_rect_dst.x)
                start_point.x = mouse.x;
            else
                start_point.x = mapping_rect_dst.x;
        }
        else {
            start_point.x = mapping_rect_dst.x+mapping_rect_dst.width;
        }
        if(mouse.y < mapping_rect_dst.y+mapping_rect_dst.height) {
            if(mouse.y > mapping_rect_dst.y)
                start_point.y = mouse.y;
            else
                start_point.y = mapping_rect_dst.y;
        }
        else {
            start_point.y = mapping_rect_dst.y+mapping_rect_dst.height;
        }
        ofPoint newstart;
        newstart.x = (start_point.x-mapping_rect_dst.x)/mapping_rect_dst.width;
        newstart.y = (start_point.y-mapping_rect_dst.y)/mapping_rect_dst.height;
        MappingController::getInstance().getProjector(0)->setStartPoint(newstart);

    }

    if(use_cam) {
        ofPoint camera_save[4] = MappingController::getInstance().getProjector(0)->getCamera();
        bool cam_changed = false;
        for (uint i = 0; i < 4; i++) {

            if (camera[i].bBeingDragged == true){

                cam_changed = true;

                camera[i].x = mouse.x;
                camera[i].y = mouse.y;

                camera_save[i].x = (camera[i].x-mapping_rect_dst.x)/mapping_rect_dst.width;
                camera_save[i].y = (camera[i].y-mapping_rect_dst.y)/mapping_rect_dst.height;

            }
        }
        if(cam_changed) {
            MappingController::getInstance().getProjector(0)->setCamera(camera_save);
            ServerController::getInstance().sendMappingQuads();
        }
    }

    return CustomTab::mouseDragged(args);
}

bool FormMapping::mousePressed(ofMouseEventArgs& args) {

    ofPoint mouse(args.x,args.y);

    for (uint i = 0; i < quads_dst.size(); i++){
        for (uint j = 0; j < quads_dst[i].size(); j++){
            bool editable = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex)->editable;
            if(editable) {
                if (mouse.distance(quads_dst[i][j].asPoint()) < quads_dst[i][j].radius){
                    quads_dst[i][j].bBeingDragged = true;
                } else {
                    quads_dst[i][j].bBeingDragged = false;
                }
            }
        }
    }
    if(mouse.distance(start_point.asPoint()) < start_point.radius)
        start_point.bBeingDragged = true;
    else
        start_point.bBeingDragged = false;

    if(use_cam) {
        for (uint i = 0; i < camera.size(); i++) {
            if (mouse.distance(camera[i].asPoint()) < camera[i].radius){
                camera[i].bBeingDragged = true;
            } else {
                camera[i].bBeingDragged = false;
            }
        }
    }

    return CustomTab::mousePressed(args);
}

bool FormMapping::mouseReleased(ofMouseEventArgs &args) {

    for (uint i = 0; i < quads_dst.size(); i++){
        for (uint j = 0; j < quads_dst[i].size(); j++){
            quads_dst[i][j].bBeingDragged = false;
        }
    }
    start_point.bBeingDragged = false;
    if(use_cam) {
        for (uint i = 0; i < camera.size(); i++){
            camera[i].bBeingDragged = false;
        }
    }

    return CustomTab::mouseReleased(args);
}
