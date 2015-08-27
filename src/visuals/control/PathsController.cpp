#include "PathsController.h"

#include "EmptyPaths.h"
#include "CtPaintingLayer.h"
#include "CtControlLayer.h"
#include "BricksLayer.h"

using namespace guardacaso;

PathsController::PathsController() {}

void PathsController::setup(float width, float height){

    xml_paths = "sessions/last/paths.xml";

    paths.clear();
    active_path = -1;
	
    addPaths(CustomPaths_ptr(new EmptyPaths("empty")));

    /********* ADD NEW PATHS HERE ********************/
    /*__________________|||_____________________________*/
    /*__________________|||_____________________________*/
    /*_________________\|||/____________________________*/
    /*__________________\|/_____________________________*/
    /*___________________'______________________________*/

    addPaths(CustomPaths_ptr(new CtControlLayer("ct-control")));
    addPaths(CustomPaths_ptr(new CtPaintingLayer("ct-painting")));
    addPaths(CustomPaths_ptr(new BricksLayer("bricks")));

    /*___________________.______________________________*/
    /*__________________/|\_____________________________*/
    /*_________________/|||\____________________________*/
    /*__________________|||_____________________________*/
    /*__________________|||_____________________________*/
	
    setActivePath(0);

    setupPaths();

    output = ofPtr<ofFbo>(new ofFbo());
    output->allocate(width, height, GL_RGB);

}

void PathsController::setupPaths(){
    ofxXmlSettings_ptr xml = ofxXmlSettings_ptr(new ofxXmlSettings());
    xml->clear();
    if( xml->loadFile(xml_paths) ){
        reloadPaths(xml);
    }else{
        cout << "unable to load xml file " << xml_paths << endl;
    }

}

void PathsController::reloadPaths(ofPtr<ofxXmlSettings> xml) {

    xml->pushTag("pathsconfig", 0);

        for(unsigned int i = 0; i < paths.size(); i++){

            CustomPaths_ptr p = getPath(i);

            if(xml->getAttribute(p->getName(), "active", 0, 0)){
                xml->pushTag(p->getName(), 0);

                    xml->deserialize(p->getSettings());

                xml->popTag();
                setActivePath(i);
            }
        }

    xml->popTag();

}

void PathsController::savePaths() {

    ofxXmlSettings xml;

    xml.clear();

    xml.addTag("pathsconfig");
    xml.pushTag("pathsconfig", 0);

        CustomPaths_ptr p = getActivePath();

        if(p){
            xml.addTag(p->getName());
            if(p->isActive().get()){
                xml.addAttribute(p->getName(), "active", 1, 0);
            }
            xml.pushTag(p->getName(), 0);

            xml.serialize(p->getSettings());

            xml.popTag();

        }

    xml.popTag();

    xml.saveFile(xml_paths);

}

void PathsController::update(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients) {

    ofSetColor(255);

    CustomPaths_ptr active = getActivePath();
    if(active){
        active->update(projector, clients);
    }

    drawOutput(projector, clients);
	
}

void PathsController::drawOutput(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr>& clients) {

    if(getActivePath()){
        output->begin();
        ofClear(0,0,0,0);

        ofSetColor(255);
        //for(int i = p->shapeCount()-1; i >= 0; i--) {
            getActivePath()->draw(projector, clients);
        //}
        output->end();
    }else{
        ofLogWarning("PathsController: draw()", "no active path style, chose one from the paths tab");
    }
}

void PathsController::addPaths(CustomPaths_ptr c) {

    paths.push_back(c);
	
}

void PathsController::setActivePath(string paths_name) {

    for(uint i = 0; i < paths.size(); i++) {
        if(paths[i]->getName() == paths_name) {
            setActivePath(i);
            break;
        }
    }

}

void PathsController::setActivePath(int index) {

    if(active_path >= 0){
        getActivePath()->idle();
        getActivePath()->isActive().set(false);
    }

    active_path = index;

    if(!getActivePath()->setupDone()){
        getActivePath()->setup();
        getActivePath()->setupDone().set(true);
    }

    getActivePath()->isActive().set(true);
    getActivePath()->resume();
}

vector<string> PathsController::getPathsNames() {
    vector<string> names;
    names.clear();
    for(uint i = 0; i < paths.size(); i++) {
        names.push_back(paths.at(i)->getName());
    }
    return names;
}

CustomPaths_ptr PathsController::getActivePath() {

    if(active_path >= 0 && active_path < (int)paths.size()){
        return paths.at(active_path);
    }else{
        ofLogError("PathsController: getActivePath()", "could not get active path with index " + ofToString(active_path.get()) + ".");
        return CustomPaths_ptr();
    }

}

CustomPaths_ptr PathsController::getPath(int index) {

    if(index < (int)paths.size())
        return paths.at(index);
    else
        return CustomPaths_ptr();

}

void PathsController::activePathChanged(int& index) {
    if(active_path.get() != index){
        setActivePath(index);
    }
}

ofPtr<ofFbo> PathsController::getOutput(){
    return output;
}

PathsController::~PathsController(){}
