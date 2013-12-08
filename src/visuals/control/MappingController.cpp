#include "MappingController.h"
#include "Visuals.h"
#include "MappingQuad.h"
#include "PathsController.h"

using namespace guardacaso;

MappingController& MappingController::getInstance() {
    static MappingController instance;
    return instance;
}

MappingController::MappingController() {

    projectors.clear();

    //we set matrix to the default - 0 translation
    //and 1.0 scale for x y z and w
    for(int i = 0; i < 16; i++)
    {
        if(i % 5 != 0) matrix[i] = 0.0;
        else matrix[i] = 1.0;
    }
	
	plane[0] = ofPoint(0, 0, 0);
    plane[1] = ofPoint(1, 0, 0);
    plane[2] = ofPoint(1, 1, 0);
    plane[3] = ofPoint(0, 1, 0);

    is_cal = false;
    cal_border = 0.05;
    cal_grey = 100;

    controlpoint = ofPoint(20,20);

}

void MappingController::update() {

    for(uint i = 0; i < projectors.size(); i++) {
        projectors[i].update();
    }

}

void MappingController::drawArea(Projector *p) {

    float output_w = Visuals::get().outputWidth();
    float output_h = Visuals::get().outputHeight();

    for(int i = p->quadCount()-1; i >= 0; i--) {

        MappingQuad_ptr q = p->getQuad(i);

        if(q) {

            ofPolyline line = Visuals::get().outlinesRaw()->at(i);

            ofPushMatrix();

            glMultMatrixf(q->matrix_dst_norm.getPtr());

            if(q->nature == "window") {
                ofSetColor(0);
            }
            else {
                ofSetColor(255);
            }

            ofFill();

            if(line.size()!= 4) {

                q->polyline.draw();

            }
            else {

                glBegin(GL_QUADS);

                    glVertex3f(0, 0, 0);
                    glVertex3f(output_w, 0, 0);
                    glVertex3f(output_w, output_h, 0);
                    glVertex3f(0, output_h, 0);

               glEnd();
            }
            ofPopMatrix();

        }
    }

}

void MappingController::drawPaths(Projector *p) {

    PathsController::getInstance().draw();

}

void MappingController::drawImages(Projector *p) {

    float output_w = Visuals::get().outputWidth();
    float output_h = Visuals::get().outputHeight();

    ofSetColor(255);

    for(int i = p->quadCount()-1; i >= 0; i--) {

        MappingQuad_ptr q = p->getQuad(i);

        ofPushMatrix();

        glMultMatrixf(q->matrix_dst_norm.getPtr());

        if(q->nature == "picture") {

            ofSetColor(255);
            ofFill();

            q->image.bind();

                glBegin(GL_QUADS);

                if(Visuals::get().isUsingComplexQuads()) {
                    glTexCoord2f(q->src[0].x*q->image.getWidth(), q->src[0].y*q->image.getHeight());		glVertex3f(0, 0, 0);
                    glTexCoord2f(q->src[1].x*q->image.getWidth(), q->src[1].y*q->image.getHeight());		glVertex3f(output_w, 0, 0);
                    glTexCoord2f(q->src[2].x*q->image.getWidth(), q->src[2].y*q->image.getHeight());		glVertex3f(output_w, output_h, 0);
                    glTexCoord2f(q->src[3].x*q->image.getWidth(), q->src[3].y*q->image.getHeight());		glVertex3f(0, output_h, 0);
                }
                else {
                    glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);
                    glTexCoord2f(q->image.getWidth(), 0);		glVertex3f(output_w, 0, 0);
                    glTexCoord2f(q->image.getWidth(), q->image.getHeight());		glVertex3f(output_w, output_h, 0);
                    glTexCoord2f(0, q->image.getHeight());		glVertex3f(0, output_h, 0);
                }

                glEnd();

            q->image.unbind();

        }

        glPopMatrix();

    }

}

void MappingController::draw(int projector_id) {

    Projector* p = getProjector(projector_id);

    drawPaths(p);
    drawImages(p);

//    drawArea(p);

    //draw white borders around windows for calibration
    if(is_cal) drawCalibration(p);

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    ofEnableAlphaBlending();

}

void MappingController::setCalibrating(bool calibrate) {
    is_cal = calibrate;
}

void MappingController::drawCalibration(Projector* p) {

    ofEnableAlphaBlending();

    for(int i = p->quadCount()-1; i >= 0; i--) {

       MappingQuad_ptr q = p->getQuad(i);

        if(q) {

            if(q->nature == "window") {

                ofSetColor(0);
                ofNoFill();

                Visuals::get().outlinesRaw()->at(i).draw();

            }

            ofSetColor(cal_grey);
            ofNoFill();
            ofSetLineWidth(cal_border);

            Visuals::get().outlinesRaw()->at(i).draw();

        }
    }

}

ofParameter<bool> MappingController::getCalibrating() {
    return is_cal;
}

void MappingController::setCalBorder(float border) {
    cal_border = border;
}

void MappingController::setCalGrey(int grey) {
    cal_grey = grey;
}

ofParameter<float> MappingController::getCalBorder() {
    return cal_border;
}

ofParameter<int> MappingController::getCalGrey() {
    return cal_grey;
}

void MappingController::addProjector() {

    projectors.push_back(Projector());

}

Projector* MappingController::getProjector(int id) {
    if(id < (int)projectors.size()) {
        return &(projectors[id]);
    }
    else {
        cout << "ERROR: mappingcontroller: trying to get projector " << id << " but projectors size is " << projectors.size() << endl;
        return 0;
    }
}

ofPoint MappingController::getPointInMappedArea(ofPoint last_p, ofPoint next_p) {

    ofPoint last_p_norm(last_p.x/Visuals::get().outputWidth(), last_p.y/Visuals::get().outputHeight());
    ofPoint res_norm;
    ofPoint res = next_p;
    if(next_p.x < 0 || next_p.y < 0)
        res = last_p;
    if(next_p.x > Visuals::get().outputWidth() || next_p.y > Visuals::get().outputHeight())
        res = last_p;

    vector<MappingQuad_ptr> windows_next_inside;
    vector<MappingQuad_ptr> paintings_next_inside;
    vector<MappingQuad_ptr> paintings_last_inside;

    windows_next_inside.clear();
    paintings_next_inside.clear();
    paintings_last_inside.clear();

    res_norm.x = res.x/Visuals::get().outputWidth();
    res_norm.y = res.y/Visuals::get().outputHeight();

    for(uint i = 0; i < getProjector(0)->quadCount(); i++) {

        MappingQuad_ptr mq = getProjector(0)->getQuad(i);

        if(mq) {

            vector<ofPoint> polyline = mq->polyline.getVertices();

//            if(mq->triangle.isPointInsidePolygon(&polyline[0],polyline.size(),res_norm)) {
            if(mq->polyline.inside(res_norm)) {

                if(mq->nature == "window")
                    windows_next_inside.push_back(mq);

                if(mq->nature == "painting")
                    paintings_next_inside.push_back(mq);
            }

            if(mq->nature == "painting") {
//                if(mq->triangle.isPointInsidePolygon(&polyline[0],polyline.size(),last_p_norm)) {
                if(mq->polyline.inside(last_p_norm)) {
                    paintings_last_inside.push_back(mq);
                }
            }
        }
    }

    if(paintings_next_inside.size() == 0) {

        //next point is outside of mapped area
        //find point between last and next point on the edge of the mapped area

        //1. find intersections between last and next point on the paintings the last point is in
        //2. chose intersection that is nearest to the next point

        float dist_res_nextp = 1000000;

        if(paintings_last_inside.size() == 0) {
            cout << "next and last point are in no paintings" << endl;
            res = getProjector(0)->getStartPoint();
        }

        for(int i = 0; i < paintings_last_inside.size(); i++) {

            MappingQuad_ptr mq = paintings_last_inside.at(i);

            res_norm.x = res.x/Visuals::get().outputWidth();
            res_norm.y = res.y/Visuals::get().outputHeight();

            ofPoint corrected_norm = intersectionPointPolyline(last_p_norm, res_norm, mq->polyline);
            ofPoint corrected(corrected_norm.x*Visuals::get().outputWidth(), corrected_norm.y*Visuals::get().outputHeight());
            float dist = ofDist(next_p.x, next_p.y, corrected.x, corrected.y);
            if(dist<dist_res_nextp) {
                dist_res_nextp = dist;
                res = corrected;
            }

        }


    }
    if(windows_next_inside.size()>0) {
        //next point is in a window area where it is not allowed
        //find point between last and next point on the edge of the window

        //1. find intersections between last and next point windows the next point is in
        //2. chose intersection that is nearest to the next point

        float dist_res_nextp = 1000000;

        for(int i = 0; i < windows_next_inside.size(); i++) {

            MappingQuad_ptr mq = windows_next_inside.at(i);

            res_norm.x = res.x/Visuals::get().outputWidth();
            res_norm.y = res.y/Visuals::get().outputHeight();

            ofPoint corrected_norm = intersectionPointPolyline(last_p_norm, res_norm, mq->polyline);
            ofPoint corrected(corrected_norm.x*Visuals::get().outputWidth(), corrected_norm.y*Visuals::get().outputHeight());
            float dist = ofDist(next_p.x, next_p.y, corrected.x, corrected.y);
            if(dist<dist_res_nextp) {
                dist_res_nextp = dist;
                res = corrected;
            }

        }
    }

    //TODO: check that there is only mapped area between last and next point when they are not in the same polyline

    return res;
}

ofPoint MappingController::getIntersection(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4) {
    ofPoint p(0,0);
    float denom = (p1.x-p2.x)*(p3.y-p4.y) - (p1.y-p2.y)*(p3.x-p4.x);
    if(denom != 0) {
        p.x = ((p1.x*p2.y-p1.y*p2.x)*(p3.x-p4.x)-(p3.x*p4.y-p3.y*p4.x)*(p1.x-p2.x))/denom;
        p.y = ((p1.x*p2.y-p1.y*p2.x)*(p3.y-p4.y)-(p3.x*p4.y-p3.y*p4.x)*(p1.y-p2.y))/denom;
    }
    return p;
}

bool MappingController::isLeft(ofPoint p1, ofPoint p2, ofPoint p_test){
     return ((p2.x - p1.x)*(p_test.y - p1.y) - (p2.y - p1.y)*(p_test.x - p1.x)) > 0;
}

bool MappingController::isOnLine(ofPoint p1, ofPoint p2, ofPoint p_test){
    return ((p2.x - p1.x)*(p_test.y - p1.y) - (p2.y - p1.y)*(p_test.x - p1.x)) < 1;
}

ofPoint MappingController::intersectionPointPolyline(ofPoint last_p, ofPoint next_p, ofPolyline polyline) {

    uint i, j=polyline.size()-1;

    ofPoint poly[polyline.size()];

    for (i=0; i<polyline.size(); i++) {
        poly[i] = polyline.getVertices().at(i);
    }

    vector<ofPoint> intersections;
    intersections.clear();

    for (i=0; i<polyline.size(); i++) {

        if(isLeft(poly[i],poly[j], last_p) != isLeft(poly[i],poly[j], next_p)
                && isLeft(next_p,last_p, poly[i]) != isLeft(next_p, last_p, poly[j])) {
            ofPoint intersection = getIntersection(last_p, next_p, poly[i], poly[j]);
            ofPoint last_inter_norm = (intersection-last_p).normalize();
            if(last_p.x<1)
                intersections.push_back(intersection-last_inter_norm*1/Visuals::get().outputWidth());
            else
                intersections.push_back(intersection-last_inter_norm);
        }
        j=i;
    }

    ofPoint min_dif_p = next_p;
    float min_dif = 100000;
    for(uint i = 0; i<intersections.size(); i++) {
        float dist = last_p.distance(intersections.at(i));
        if(dist < min_dif) {
            min_dif_p = intersections.at(i);
            min_dif = dist;
        }
    }

    return min_dif_p;
}

void MappingController::clear() {
    projectors.clear();
}
