#include "Visuals.h"
#include "MappingController.h"
#include "ControlWindow.h"
#include "PathsController.h"
#include "Stuff.h"

using namespace guardacaso;

typedef std::shared_ptr<MappingQuad> MappingQuad_ptr;
typedef std::shared_ptr<ofPolyline> ofPolyline_ptr;

Visuals& Visuals::get() {
    static Visuals instance;
    return instance;
}

Visuals::Visuals() {
    xml_mapping = "sessions/last/mapping.xml";
    xml_other = "sessions/last/other.xml";
    xml_paths = "sessions/last/paths.xml";
    svg_mapping = "sessions/last/mapping.svg";
    png_mapping = "sessions/last/mapping.png";

    use_complex_quads = false;
    control_w = 0;
    control_h = 0;
    output_w = 0;
    output_h = 0;
    content_w = 0;
    content_h = 0;

    control_left = true;

    _svg = ofxSVG_ptr(new ofxSVG());
    _outlines = ofPolylines_ptr(new vector<ofPolyline>());
    _outlines_raw = ofPolylines_ptr(new vector<ofPolyline>());
    _paths = ofPaths_ptr(new vector<ofPath>());
    _outlines->clear();
    _outlines_raw->clear();
    _paths->clear();
}

//--------------------------------------------------------------
void Visuals::setupMapping(){
    reloadMapping(xml_mapping);
}

//--------------------------------------------------------------
void Visuals::setupPaths(){
    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(xml_paths) ){
        reloadPaths(xml);
    }else{
        cout << "unable to load xml file " << xml_paths << endl;
    }

}

//--------------------------------------------------------------
void Visuals::setupOther(){
    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(xml_other) ){
        reloadOther(xml);
    }else{
        cout << "unable to load xml file" << xml_other << endl;
    }

}

void Visuals::update(){
    PathsController::getInstance().update();
    MappingController::getInstance().update();
}

void Visuals::reloadMapping(string xml_path) {
    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(xml_path) ){
        reloadMapping(xml);
    }else{
        cout << "unable to load xml file " << xml_path << endl;
    }
}

void Visuals::reloadMapping(ofxXmlSettings_ptr xml) {

    MappingController::getInstance().clear();

    xml->pushTag("mapping", 0);

    use_complex_quads = xml->getValue("usecomplexquads", 0);

    xml->pushTag("content", 0);

    content_w		= xml->getValue("width", 640.);
    content_h		= xml->getValue("height", 480.);

    xml->popTag();
    xml->pushTag("output", 0);
	
    output_w		= xml->getValue("width", 1024.);
    output_h		= xml->getValue("height", 768.);

    xml->popTag();
    xml->pushTag("control", 0);

    control_w		= xml->getValue("width", 1024.);
    control_h		= xml->getValue("height", 768.);

    xml->popTag();

    int projector_count = xml->getNumTags("projector");

    for(int i = 0; i < projector_count; i++) {

        xml->pushTag("projector", i);

        MappingController::getInstance().addProjector();

        int quad_count = xml->getNumTags("quad");

        for (int j = 0; j < quad_count; j++){

            string type = xml->getAttribute("quad","type","window",j);
            bool showframe = xml->getAttribute("quad","frame",false,j);

            MappingQuad_ptr mq = MappingController::getInstance().getProjector(i)->addQuad(type);

            mq->showframe = showframe;
            mq->newpos = true;

            xml->pushTag("quad", j);

                if(type == "picture") {
                    string url = xml->getValue("url", "images/notfound.png");
                    mq->image.clear();
                    mq->image.loadImage(url);
                    mq->img_src = url;
                }

                if(use_complex_quads) {
                    xml->pushTag("src", 0);

                        xml->pushTag("lefttop", 0);
                            mq->src[0].x = xml->getValue("x", 0.);
                            mq->src[0].y = xml->getValue("y", 0.);
                        xml->popTag();
                        xml->pushTag("righttop", 0);
                            mq->src[1].x = xml->getValue("x", 1.);
                            mq->src[1].y = xml->getValue("y", 0.);
                        xml->popTag();
                        xml->pushTag("rightbottom", 0);
                        mq->src[2].x = xml->getValue("x", 1.);
                        mq->src[2].y = xml->getValue("y", 1.);
                        xml->popTag();
                        xml->pushTag("leftbottom", 0);
                            mq->src[3].x = xml->getValue("x", 0.);
                            mq->src[3].y = xml->getValue("y", 1.);
                        xml->popTag();

                    xml->popTag();
                }

                xml->pushTag("dst", 0);

                    xml->pushTag("lefttop", 0);
                        mq->dst[0].x = xml->getValue("x", 0.);
                        mq->dst[0].y = xml->getValue("y", 0.);
                    xml->popTag();
                    xml->pushTag("righttop", 0);
                    mq->dst[1].x = xml->getValue("x", 1.);
                        mq->dst[1].y = xml->getValue("y", 0.);
                    xml->popTag();
                    xml->pushTag("rightbottom", 0);
                    mq->dst[2].x = xml->getValue("x", 1.);
                    mq->dst[2].y = xml->getValue("y", 1.);
                    xml->popTag();
                    xml->pushTag("leftbottom", 0);
                        mq->dst[3].x = xml->getValue("x", 0.);
                        mq->dst[3].y = xml->getValue("y", 1.);
                    xml->popTag();

                xml->popTag();

                xml->pushTag("polyline");

                    mq->polyline.clear();

                    int point_count = xml->getNumTags("point");

                    for(int k = 0; k < point_count; k++) {
                        xml->pushTag("point",k);
                            ofPoint point(xml->getValue("x", 0.), xml->getValue("y", 0.));
                            mq->polyline.addVertex(point);
                        xml->popTag();
                    }

                    mq->polyline.close();

                xml->popTag();

            xml->popTag();

        }

        updateOutlines();

        int point_count = xml->getNumTags("point");

        for(int j = 0; j < point_count; j++) {

            if(xml->getAttribute("point","type","start",j) == "start") {
                xml->pushTag("point", j);
                ofPoint startpoint;
                startpoint.x = xml->getValue("x",0.);
                startpoint.y = xml->getValue("y",0.);
                MappingController::getInstance().getProjector(i)->setStartPoint(startpoint);
            }

            xml->popTag();
        }


        xml->popTag();

    }

}

void Visuals::importSvg() {

    importSvg(svg_mapping);

}

void Visuals::importSvg(string svg) {

    reloadSvg(svg);

    int quadcount = MappingController::getInstance().getProjector(0)->quadCount();
    int outlinescount = _outlines_raw->size();

    bool quads_match = quadcount == outlinescount;

    if(!quads_match) {
        MappingController::getInstance().getProjector(0)->removeAllQuads();
    }

    for (int j = 0; j < outlinescount; j++) {

        ofColor fill_col = _paths->at(j).getFillColor();
        string type = (fill_col.getLightness()>255/2) ? "painting" : "window";
        ofPolyline l = _outlines_raw->at(j);
        ofRectangle bounding = l.getBoundingBox();

        MappingQuad_ptr mq;

        if(!quads_match) {

            mq = MappingController::getInstance().getProjector(0)->addQuad(type, false, false);
            mq->showframe = false;
            if(mq->nature == "picture") {
                string url = "images/notfound.png";
                mq->image.loadImage(url);
                mq->img_src = url;
            }
        }
        else {
            mq = MappingController::getInstance().getProjector(0)->getQuad(j);
        }

        mq->nature = type;

        if(l.size() == 4) {
            mq->dst[0].x = l[0].x/output_w;
            mq->dst[0].y = l[0].y/output_h;
            mq->dst[1].x = l[1].x/output_w;
            mq->dst[1].y = l[1].y/output_h;
            mq->dst[2].x = l[2].x/output_w;
            mq->dst[2].y = l[2].y/output_h;
            mq->dst[3].x = l[3].x/output_w;
            mq->dst[3].y = l[3].y/output_h;
        }
        else {
            mq->dst[0].x = bounding.x/output_w;
            mq->dst[0].y = bounding.y/output_h;
            mq->dst[1].x = (bounding.x+bounding.width)/output_w;
            mq->dst[1].y = bounding.y/output_h;
            mq->dst[2].x = (bounding.x+bounding.width)/output_w;
            mq->dst[2].y = (bounding.y+bounding.height)/output_h;
            mq->dst[3].x = bounding.x/output_w;
            mq->dst[3].y = (bounding.y+bounding.height)/output_h;
        }

        mq->polyline.clear();

        for(uint k = 0; k < l.size(); k++) {
            mq->polyline.addVertex(l[k].x/outputWidth(), l[k].y/outputHeight());
        }

        mq->polyline.close();
        mq->newpos = true;

    }

    MappingController::getInstance().update();

    cout << "visuals::importsvg done " << endl;

}

void Visuals::reloadSvg(string file) {

    _svg = ofxSVG_ptr(new ofxSVG());
    _svg->load(file);

    _outlines_raw->clear();
    _outlines->clear();
    _paths->clear();

    int paths_num = 0;

    for(int j = 0; j<_svg->getNumPath(); j++) {
        _paths->push_back(_svg->getPathAt(j));
//        _outlines_raw->push_back(Stuff::ofPathToOfPolyline(_svg->getPathAt(j), true));
        _outlines_raw->push_back(_svg->getPathAt(j).getOutline().at(0));
        _outlines->push_back(_outlines_raw->at(paths_num).getResampledBySpacing(1));
        paths_num++;
    }

}

void Visuals::reloadLinesFromRaw() {

    for (int i = 0; i < (int)_outlines_raw->size(); i++){

        _outlines->at(i) = _outlines_raw->at(i).getResampledBySpacing(1);

    }

}

void Visuals::reloadPaths(ofxXmlSettings_ptr xml) {

    xml->pushTag("pathsconfig", 0);

    for(int i = 0; i < 1; i++) {


        xml->pushTag("deck", i);

            CustomPaths_ptr p;

            xml->pushTag("paths1", 0);

                PathsController::getInstance().setActivePath(xml->getValue("name", "empty"),false);
                p = PathsController::getInstance().getActivePath();

                reloadPathsData(p, xml);

            xml->popTag();

        xml->popTag();

    }

    xml->popTag();

}

void Visuals::reloadOther(ofxXmlSettings_ptr xml) {

    xml->pushTag("settings", 0);

    xml->popTag();

}

void Visuals::reloadPathsData(CustomPaths_ptr p, ofxXmlSettings_ptr xml) {

    p->setSlider1_vertical(xml->getValue("sl1", 0.));
    p->setSlider2_vertical(xml->getValue("sl2", 0.));
    p->setSlider3_vertical(xml->getValue("sl3", 0.));
    p->setSlider1_rotate(xml->getValue("rsl1", 0.));
    p->setSlider2_rotate(xml->getValue("rsl2", 0.));
    p->setSlider3_rotate(xml->getValue("rsl3", 0.));
    p->setButton1_top(xml->getValue("btn1-1", 0));
    p->setButton2_top(xml->getValue("btn2-1", 0));
    p->setButton3_top(xml->getValue("btn3-1", 0));
    p->setButton1_bottom(xml->getValue("btn1-2", 0));
    p->setButton2_bottom(xml->getValue("btn2-2", 0));
    p->setButton3_bottom(xml->getValue("btn3-2", 0));
    p->setColor(ofColor(
                    xml->getAttribute("color", "r", 255),
                    xml->getAttribute("color", "g", 255),
                    xml->getAttribute("color", "b", 255),
                    xml->getAttribute("color", "a", 255)));

}

void Visuals::saveMappingDefault() {
    saveMapping(xml_mapping, svg_mapping, png_mapping);
}

void Visuals::saveMapping(string path, string path_svg, string path_png) {

    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("mapping");

    xml.pushTag("mapping", 0);

        xml.addValue("usecomplexquads", use_complex_quads);

        xml.addTag("content");
        xml.pushTag("content", 0);
            xml.addValue("width", (int)content_w);
            xml.addValue("height", (int)content_h);
        xml.popTag();
        xml.addTag("output");
        xml.pushTag("output", 0);
            xml.addValue("width", (int)output_w);
            xml.addValue("height", (int)output_h);
        xml.popTag();
        xml.addTag("control");
        xml.pushTag("control", 0);
            xml.addValue("width", (int)control_w);
            xml.addValue("height", (int)control_h);
        xml.popTag();

        xml.addTag("projector");

        xml.pushTag("projector", 0);

            int i = 0;
            for(uint j = 0; j < MappingController::getInstance().getProjector(0)->quadCount(); j++) {

                MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(j);

                if(mq) {

                    xml.addTag("quad");

                    xml.addAttribute("quad","type",mq->nature, i);
                    xml.addAttribute("quad","frame",mq->showframe, i);
                    xml.pushTag("quad", i);

                        if(mq->nature == "picture") {
                            xml.addValue("url", mq->img_src);
                        }

                        xml.addTag("src");
                        xml.pushTag("src", 0);
                            xml.addTag("lefttop");
                            xml.pushTag("lefttop", 0);
                                xml.addValue("x", mq->src[0].x);
                                xml.addValue("y", mq->src[0].y);
                            xml.popTag();
                            xml.addTag("righttop");
                            xml.pushTag("righttop", 0);
                                xml.addValue("x", mq->src[1].x);
                                xml.addValue("y", mq->src[1].y);
                            xml.popTag();
                            xml.addTag("rightbottom");
                            xml.pushTag("rightbottom", 0);
                                xml.addValue("x", mq->src[2].x);
                                xml.addValue("y", mq->src[2].y);
                            xml.popTag();
                            xml.addTag("leftbottom");
                            xml.pushTag("leftbottom", 0);
                                xml.addValue("x", mq->src[3].x);
                                xml.addValue("y", mq->src[3].y);
                            xml.popTag();
                        xml.popTag();

                        xml.addTag("dst");
                        xml.pushTag("dst", 0);
                            xml.addTag("lefttop");
                            xml.pushTag("lefttop", 0);
                                xml.addValue("x", mq->dst[0].x);
                                xml.addValue("y", mq->dst[0].y);
                            xml.popTag();
                            xml.addTag("righttop");
                            xml.pushTag("righttop", 0);
                                xml.addValue("x", mq->dst[1].x);
                                xml.addValue("y", mq->dst[1].y);
                            xml.popTag();
                            xml.addTag("rightbottom");
                            xml.pushTag("rightbottom", 0);
                                xml.addValue("x", mq->dst[2].x);
                                xml.addValue("y", mq->dst[2].y);
                            xml.popTag();
                            xml.addTag("leftbottom");
                            xml.pushTag("leftbottom", 0);
                                xml.addValue("x", mq->dst[3].x);
                                xml.addValue("y", mq->dst[3].y);
                            xml.popTag();
                        xml.popTag();

                        xml.addTag("polyline");
                        xml.pushTag("polyline",0);

                            for(uint k = 0; k < mq->polyline.size(); k++) {
                                xml.addTag("point");
                                xml.pushTag("point",k);
                                    xml.addValue("x", mq->polyline[k].x);
                                    xml.addValue("y", mq->polyline[k].y);
                                xml.popTag();
                            }

                        xml.popTag();

                    xml.popTag();

                    i++;

                }
            }

            xml.addTag("point");
            xml.addAttribute("point","type","start", 0);
            xml.pushTag("point", 0);
                xml.addValue("x", MappingController::getInstance().getProjector(0)->getStartPoint().x);
                xml.addValue("y", MappingController::getInstance().getProjector(0)->getStartPoint().y);
            xml.popTag();

        xml.popTag();

    xml.popTag();

    xml.saveFile(path);

    saveMappingAsSvg(path_svg);

}

void Visuals::saveMappingAsSvg() {
    saveMappingAsSvg(svg_mapping);
}

void Visuals::saveMappingAsSvg(string path) {
    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("svg");
    xml.addAttribute("svg","id","svg2",0);
    xml.addAttribute("svg","xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#",0);
    xml.addAttribute("svg","xmlns","http://www.w3.org/2000/svg",0);
    xml.addAttribute("svg","width",outputWidth(),0);
    xml.addAttribute("svg","height",outputHeight(),0);
    xml.addAttribute("svg","version","1.1",0);
    xml.addAttribute("svg","xmlns:cc","http://creativecommons.org/ns#",0);
    xml.addAttribute("svg","xmlns:dc","http://purl.org/dc/elements/1.1/",0);
    xml.pushTag("svg", 0);
//        xml.addTag("metadata");
//        xml.addAttribute("metadata","id","metadata7");
//        xml.pushTag("metadata");
//        xml.popTag();
        xml.addTag("g");
        xml.addAttribute("g", "id", "layer1",0);
        xml.pushTag("g");

            int i = 0;
            for(uint j = 0; j < MappingController::getInstance().getProjector(0)->quadCount(); j++) {

                MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(j);

                if(mq) {

                    xml.addTag("path");
                    stringstream id_sstr;
                    id_sstr << "path" << i;
                    xml.addAttribute("path", "id", id_sstr.str(), i);
                    if(mq->nature == "painting") {
                        xml.addAttribute("path", "fill", "#ffffff", i);
                    }
                    else if(mq->nature == "window") {
                        xml.addAttribute("path", "fill", "#000000", i);
                    }
                    stringstream path_sstr;
                    path_sstr << "m";
                    ofPoint last_p;
                    for(uint k = 0; k < mq->polyline.size(); k++) {
                        ofPoint cur_p = ofPoint(mq->polyline[k].x*outputWidth(), mq->polyline[k].y*outputHeight());
                        if(k == 0)
                            path_sstr << cur_p.x << "," << cur_p.y;
                        if(k > 0)
                            path_sstr << "," << cur_p.x-last_p.x << "," << cur_p.y-last_p.y;
                        last_p = cur_p;
                    }
                    path_sstr << "z";

                    xml.addAttribute("path", "d", path_sstr.str(), i);
                    xml.addAttribute("path", "stroke", "#000000", i);

                    i++;

                }
            }

        xml.popTag();

    xml.popTag();

    xml.saveFile(path);
}

void Visuals::savePaths() {

    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("pathsconfig");
    xml.pushTag("pathsconfig", 0);

        int d_num = xml.getNumTags("deck");

        xml.addTag("deck");

        xml.addAttribute("deck", "visible", 1, d_num);

        xml.pushTag("deck", d_num);

            CustomPaths_ptr p;
            xml.addTag("paths1");
            xml.pushTag("paths1", 0);
                p = PathsController::getInstance().getActivePath();

                xml.addValue("name", p->getName());
                xml.addValue("sl1", p->getData()->sl1);
                xml.addValue("sl2", p->getData()->sl2);
                xml.addValue("sl3", p->getData()->sl3);
                xml.addValue("rsl1", p->getData()->rsl1);
                xml.addValue("rsl2", p->getData()->rsl2);
                xml.addValue("rsl3", p->getData()->rsl3);
                xml.addValue("btn1-1", p->getData()->btn1_1);
                xml.addValue("btn2-2", p->getData()->btn2_2);
                xml.addValue("btn3-1", p->getData()->btn3_1);
                xml.addValue("btn1-2", p->getData()->btn1_2);
                xml.addValue("btn2-1", p->getData()->btn2_1);
                xml.addValue("btn3-2", p->getData()->btn3_2);
                xml.addTag("color");
                xml.addAttribute("color","r",(int)p->getData()->color.get().r,0);
                xml.addAttribute("color","g",(int)p->getData()->color.get().g,0);
                xml.addAttribute("color","b",(int)p->getData()->color.get().b,0);
                xml.addAttribute("color","a",(int)p->getData()->color.get().a,0);

            xml.popTag();

        xml.popTag();

    xml.popTag();

    xml.saveFile(xml_paths);

}

void Visuals::saveOther() {

    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("settings");
    xml.pushTag("settings", 0);

    xml.popTag();

    xml.saveFile(xml_other);

}

float Visuals::contentWidth() {
	return content_w;
}

float Visuals::contentHeight() {
	return content_h;
}

float Visuals::outputWidth() {
	return output_w;
}

float Visuals::outputHeight() {
	return output_h;
}

float Visuals::controlWidth() {
    return control_w;
}

float Visuals::controlHeight() {
    return control_h;
}

void Visuals::setOutputWidth(float val) {
    output_w = val;
}

void Visuals::setOutputHeight(float val) {
    output_h = val;
}

void Visuals::setControlWidth(float val) {
    control_w = val;
}

void Visuals::setControlHeight(float val) {
    control_h = val;
}

void Visuals::setUseComplexQuads(bool complex) {
    use_complex_quads = complex;
}

bool Visuals::isUsingComplexQuads() {
    return use_complex_quads;
}

ofParameter<bool> Visuals::controlLeft() {
    return control_left;
}

void Visuals::setControlLeft(bool setleft) {
    control_left = setleft;
}

ofPoint Visuals::controlPoint() {
    if(controlLeft())
        return ofPoint(0,0);
    else
        return ofPoint(outputWidth(),0);
}

ofxSVG_ptr Visuals::svg() {
    return _svg;
}

ofPolylines_ptr Visuals::outlines() {
    return _outlines;
}

void Visuals::updateOutlines() {

    _outlines->clear();
    _outlines_raw->clear();
    _paths->clear();

    for(int i = 0; i < (int)MappingController::getInstance().getProjector(0)->quadCount(); i++) {

        MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(i);

        _paths->push_back(ofPath());

        _outlines_raw->push_back(mq->polyline);

        for(uint j = 0; j < _outlines_raw->at(i).size(); j++) {
            _outlines_raw->at(i)[j].x *= outputWidth();
            _outlines_raw->at(i)[j].y *= outputHeight();

            //cout << _outlines_raw->at(i)[j] << endl;

            if(j == 0)
                ((ofPath)_paths->at(i)).moveTo(_outlines_raw->at(i)[j]);
            else
                ((ofPath)_paths->at(i)).lineTo(_outlines_raw->at(i)[j]);
        }

        if(mq->nature == "painting") {
            ((ofPath)_paths->at(i)).setFillColor(ofColor(255));
        }
        else if(mq->nature == "window") {
            ((ofPath)_paths->at(i)).setFillColor(ofColor(0));
        }

        _outlines->push_back(_outlines_raw->at(i).getResampledBySpacing(1));

    }

}

ofPolylines_ptr Visuals::outlinesRaw() {
    return _outlines_raw;
}

ofPaths_ptr Visuals::paths() {
    return _paths;
}

Visuals::~Visuals() {
}
