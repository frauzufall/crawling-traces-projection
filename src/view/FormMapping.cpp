#include "FormMapping.h"
#include "MappingController.h"

using namespace guardacaso;

FormMapping::FormMapping(): CustomTab() {

    setup("MAPPING FORMS");

    img_col = ofColor::fromHex(0x2da1e3);
    pic_col = ofColor(200,0,200);
    win_col = ofColor(150,150,150);

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

    for (int i = 0; i < p->quadCount(); i++){

        ofPolyline line = p->getQuad(i)->polyline;

        quads_dst.push_back(vector<draggableVertex>());

        for(int j = 0; j < line.size(); j++) {
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

}

void FormMapping::update() {

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
    ofRect(0,0,Visuals::get().outputWidth(),Visuals::get().outputHeight());

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
            for(uint j = 0; j < quads_dst[i].size(); j++) {
                ofVertex(quads_dst[i][j].x, quads_dst[i][j].y);
            }
            ofEndShape(true);
        }
    }

    //DRAW CIRCLES

    ofSetColor(255,255,255,200);
    for (int i = quads_dst.size()-1; i >= 0; i--){

        for(uint j = 0; j < quads_dst[i].size(); j++) {
            bool editable = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex)->editable;
            if(editable) {
                if (quads_dst[i][j].bOver) ofFill();
                else ofNoFill();
                ofCircle(quads_dst[i][j].x, quads_dst[i][j].y,6);
            }

        }
    }
    ofSetColor(0,255,255,200);
    if(start_point.bOver)
        ofFill();
    else ofNoFill();
    ofCircle(start_point.x, start_point.y,13);

}

ofColor FormMapping::getColorOf(string name) {
    if(name == "painting") return img_col;
    else if(name == "picture") return pic_col;
    else if(name == "window") return win_col;
    else return ofColor(0);
}

bool FormMapping::mouseMoved(ofMouseEventArgs& args) {

    CustomTab::mouseMoved(args);

    ofPoint mouse(args.x,args.y);
    for (uint i = 0; i < quads_dst.size(); i++){
        for (uint j = 0; j < quads_dst[i].size(); j++){
            float diffx = mouse.x - quads_dst[i][j].x;
            float diffy = mouse.y - quads_dst[i][j].y;
            float dist = sqrt(diffx*diffx + diffy*diffy);
            if (dist < quads_dst[i][j].radius){
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
}


bool FormMapping::mouseDragged(ofMouseEventArgs &args) {

    CustomTab::mouseDragged(args);

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
}

bool FormMapping::mousePressed(ofMouseEventArgs& args) {

    CustomTab::mousePressed(args);

    ofPoint mouse(args.x,args.y);

    for (uint i = 0; i < quads_dst.size(); i++){
        for (uint j = 0; j < quads_dst[i].size(); j++){
            bool editable = MappingController::getInstance().getProjector(0)->getQuad(quads_dst[i][0].quadIndex)->editable;
            if(editable) {
                float diffx = mouse.x - quads_dst[i][j].x;
                float diffy = mouse.y - quads_dst[i][j].y;
                float dist = sqrt(diffx*diffx + diffy*diffy);
                if (dist < quads_dst[i][j].radius){
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
}

bool FormMapping::mouseReleased(ofMouseEventArgs &args) {

    CustomTab::mouseReleased(args);

    ofPoint mouse(args.x,args.y);

    for (uint i = 0; i < quads_dst.size(); i++){
        for (uint j = 0; j < quads_dst[i].size(); j++){
            quads_dst[i][j].bBeingDragged = false;
        }
    }
    start_point.bBeingDragged = false;
}
