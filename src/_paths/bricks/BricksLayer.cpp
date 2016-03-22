#include "BricksLayer.h"

BricksLayer::BricksLayer(string name):CustomPaths(name) {
    settings.add(stroke1_w.set("stroke1 w", 1, 0.5, 20));
    settings.add(stroke2_w.set("stroke2 w", 1, 0.5, 20));
    settings.add(control_size.set("control size", 6, 1, 33));
    settings.add(alpha1.set("alpha1", 1, 0, 1));
    settings.add(alpha2.set("alpha2", 1, 0, 1));
    settings.add(pulse_size.set("pulse size", 1, 1, 4));
    settings.add(show_lines.set("show lines", true));
    settings.add(show_controls.set("show controls", true));
    settings.add(drawing_area_w.set("wall w", 800, 100, 2000));
    settings.add(drawing_area_h.set("wall h", 600, 50, 1000));
    settings.add(blur_shapes_set.set("reload shapes", false));
    settings.add(blur_clear.set("clear blur", true));
    settings.add(blur_scale.set("blur scale", 2, 1, 10));
    settings.add(blur_radius.set("blur radius", 5, 0, 50));
    settings.add(blur_shape.set("blur shape", .2, 0, 1));
    settings.add(blur_passes.set("blur passes", 0, 0, 10));
    settings.add(brickcols.set("brick cols",30, 1, 150));
    settings.add(brickrows.set("brickrows",10, 1, 150));
    settings.add(brickoffset.set("brick offset", 0.5, 0, 1));
    settings.add(calibration.set("calibration", false));
}

void BricksLayer::setup() {
    blur_shapes_set = false;
}

void BricksLayer::setBlurShapes(ofPtr<ofx2DMappingProjector> projector){
    blurshapes.clear();
    for(auto e : projector->getShapesByClass<ofx2DMappingContentShape>()){
        if(e->name == "drawing area"){
            ofPtr<BlurShape> bs = ofPtr<BlurShape>(new BlurShape());
            bs->width = drawing_area_w*(e->src[2][0] - e->src[0][0]);
            bs->height = drawing_area_h*(e->src[2][1] - e->src[0][1]);
            bs->blur.setup(bs->width, bs->height, blur_radius, blur_shape, blur_passes);
            bs->blur.brickCols().makeReferenceTo(brickcols);
            bs->blur.brickRows().makeReferenceTo(brickrows);
            bs->blur.brickOffset().makeReferenceTo(brickoffset);
            blurshapes.push_back(bs);
        }
    }
    blur_shapes_set = true;
}

void BricksLayer::update(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients) {
    if(!blur_shapes_set){
        setBlurShapes(projector);
    }
    for(auto blurshape : blurshapes){
        blurshape->blur.setScale(blur_scale);
    }
}

void BricksLayer::draw(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients) {

    ofEnableAlphaBlending();
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,
           GL_NICEST);
    ofNoFill();

    int i = 0;

    for(auto drawing_shape : projector->getShapesByClass<ofx2DMappingContentShape>()) {
        if(drawing_shape->name == "drawing area"){

            vector<DrawingObject_ptr>::iterator iter;
            DrawingObject_ptr c;
            for( iter = clients.begin(); iter != clients.end(); iter++) {

                c = *iter;

                if(c) {

                   blurshapes[i]->blur.begin();

                   if(blur_clear){
                       ofSetColor(0);
                       ofFill();
                       ofDrawRectangle(0,0,blurshapes[i]->width, blurshapes[i]->height);
                   }

                   ofPushMatrix();
                   ofScale(blurshapes[i]->width / projector->outputWidth(), blurshapes[i]->height / projector->outputHeight());

                    if(show_lines) {
                        ofColor col = c->getColor();
                        col.setBrightness(col.getBrightness()*1.1);

                        ofSetLineWidth(stroke1_w);
                        ofSetColor(col, col.a*alpha1);

                        ofPolyline connections_mapped;
                        for(int i = 0; i < c->getConnections().getVertices().size(); i++){
                            ofPoint p = projector->relative(c->getConnections().getVertices().at(i));
                            connections_mapped.addVertex(p);
                        }

                        connections_mapped.draw();

                        ofSetLineWidth(stroke2_w);
                        ofSetColor(col, col.a*alpha2);

                        ofPolyline line_mapped;

                        for(int i = 0; i < c->getLine().getVertices().size(); i++){
                            ofPoint p = projector->relative(c->getLine().getVertices().at(i));
                            line_mapped.addVertex(p);
                        }

                        line_mapped.draw();
                    }

                    if(show_controls) {
                        ofColor col = c->getColor();
                        col.setBrightness(col.getBrightness()*1.2);
                        ofSetColor(col);
                        ofNoFill();
                        ofSetLineWidth(3);

                        float current_time = ofGetElapsedTimef();
                        float pulse_time = (int)(c->getPulseDuration()+1);
                        float pulsing = (current_time-c->getPulseStart())/pulse_time;
                        c->getPulseVal().set(pulsing);
                        if(pulsing < 1) {
                            float tmp = fmodf(pulsing,1./pulse_time)*pulse_time;
                            float pulse_val = 1-(cos(tmp*2*PI)+1)/2;
                            ofDrawCircle(projector->relative(c->getPos()),control_size*(pulse_size*pulse_val+1));
                        }
                        else {
                            ofDrawCircle(projector->relative(c->getPos()),control_size);
                        }
                    }

                    ofPopMatrix();
                    blurshapes[i]->blur.end();

                    ofPushMatrix();

                        glMultMatrixf(drawing_shape->matrix_dst_norm.getPtr());

                        ofSetColor(255);
                        ofFill();

                        blurshapes[i]->blur.getTextureReference().bind();
                        float src_width = blurshapes[i]->width;
                        float src_height = blurshapes[i]->height;
                        float w = projector->outputWidth();
                        float h = projector->outputHeight();

//                        if(c->getLine().size()!= 4) {

//                            glBegin(GL_TRIANGLES);
//                            for (int i = 0; i < drawing_shape->triangle.nTriangles; i++){

//                                float cx = (((drawing_shape->triangle.triangles[i].a.x - drawing_shape->dst[0].x) / (drawing_shape->dst[1].x-drawing_shape->dst[0].x) * (drawing_shape->src[1].x-drawing_shape->src[0].x))+ drawing_shape->src[0].x)* src_width;
//                                float cy = (((drawing_shape->triangle.triangles[i].a.y - drawing_shape->dst[0].y) / (drawing_shape->dst[2].y-drawing_shape->dst[0].y) * (drawing_shape->src[2].y-drawing_shape->src[0].y))+ drawing_shape->src[0].y) * src_height;
//                                float ox = (drawing_shape->triangle.triangles[i].a.x - drawing_shape->dst[0].x) / (float)(drawing_shape->dst[1].x-drawing_shape->dst[0].x) * w;
//                                float oy = (drawing_shape->triangle.triangles[i].a.y - drawing_shape->dst[0].y) / (float)(drawing_shape->dst[2].y-drawing_shape->dst[0].y) * h;
//                                glTexCoord2f(cx, cy);
//                                glVertex2f(ox, oy);

//                                cx = (((drawing_shape->triangle.triangles[i].b.x - drawing_shape->dst[0].x) / (drawing_shape->dst[1].x-drawing_shape->dst[0].x) * (drawing_shape->src[1].x-drawing_shape->src[0].x))+ drawing_shape->src[0].x)* src_width;
//                                cy = (((drawing_shape->triangle.triangles[i].b.y - drawing_shape->dst[0].y) / (drawing_shape->dst[2].y-drawing_shape->dst[0].y) * (drawing_shape->src[2].y-drawing_shape->src[0].y))+ drawing_shape->src[0].y) * src_height;
//                                ox = (drawing_shape->triangle.triangles[i].b.x - drawing_shape->dst[0].x)/(float)(drawing_shape->dst[1].x-drawing_shape->dst[0].x)*w;
//                                oy = (drawing_shape->triangle.triangles[i].b.y - drawing_shape->dst[0].y)/(float)(drawing_shape->dst[2].y-drawing_shape->dst[0].y)*h;
//                                glTexCoord2f(cx, cy);
//                                glVertex2f(ox, oy);

//                                cx = (((drawing_shape->triangle.triangles[i].c.x - drawing_shape->dst[0].x) / (drawing_shape->dst[1].x-drawing_shape->dst[0].x) * (drawing_shape->src[1].x-drawing_shape->src[0].x))+ drawing_shape->src[0].x)* src_width;
//                                cy = (((drawing_shape->triangle.triangles[i].c.y - drawing_shape->dst[0].y) / (drawing_shape->dst[2].y-drawing_shape->dst[0].y) * (drawing_shape->src[2].y-drawing_shape->src[0].y))+ drawing_shape->src[0].y) * src_height;
//                                ox = (drawing_shape->triangle.triangles[i].c.x - drawing_shape->dst[0].x)/(float)(drawing_shape->dst[1].x-drawing_shape->dst[0].x)*w;
//                                oy = (drawing_shape->triangle.triangles[i].c.y - drawing_shape->dst[0].y)/(float)(drawing_shape->dst[2].y-drawing_shape->dst[0].y)*h;
//                                glTexCoord2f(cx, cy);
//                                glVertex2f(ox, oy);

//                            }
//                            glEnd();
//                        }
//                        else {

                            glBegin(GL_QUADS);

                            glTexCoord2f(drawing_shape->src[0].x*src_width, drawing_shape->src[0].y*src_height);		glVertex3f(0, 0, 0);
                            glTexCoord2f(drawing_shape->src[1].x*src_width, drawing_shape->src[1].y*src_height);		glVertex3f(w, 0, 0);
                            glTexCoord2f(drawing_shape->src[2].x*src_width, drawing_shape->src[2].y*src_height);		glVertex3f(w, h, 0);
                            glTexCoord2f(drawing_shape->src[3].x*src_width, drawing_shape->src[3].y*src_height);		glVertex3f(0, h, 0);

                           glEnd();
                        //}

                        blurshapes[i]->blur.getTextureReference().unbind();

                        if(calibration){
                            ofPath bricks;
                            bricks.setFilled(false);
                            bricks.setStrokeColor(ofColor::white);
                            bricks.setStrokeWidth(1);
                            float brick_w = projector->outputWidth()/brickcols;
                            float brick_h = projector->outputHeight()/brickrows;
                            for(int i = 0; i < brickrows; i++){
                                for(int j = 0; j < brickcols; j++){
                                    ofPoint p(j*brick_w, i*brick_h);
                                    if(i%2 == 0){
                                        p.x += brick_w*brickoffset;
                                    }
                                    bricks.rectangle(p.x,p.y,brick_w, brick_h);
                                }

                            }
                            bricks.draw();
                        }

                    ofPopMatrix();

                }

            }

        }

        i++;
    }


}

void BricksLayer::idle() {

}

void BricksLayer::resume() {

}
