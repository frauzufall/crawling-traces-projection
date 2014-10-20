#include "PathsController.h"
#include "MappingController.h"

#include "EmptyPaths.h"
#include "CtPaintingLayer.h"
#include "CtControlLayer.h"

using namespace guardacaso;

PathsController& PathsController::getInstance() {
    static PathsController instance;
    return instance;
}

PathsController::PathsController() {

    w = Visuals::get().outputWidth();
    h = Visuals::get().outputHeight();

    paths.clear();
    active_path = 0;
	
    addPaths(CustomPaths_ptr(new EmptyPaths("empty")));

    /********* ADD NEW PATHS HERE ********************/
    /*__________________|||_____________________________*/
    /*__________________|||_____________________________*/
    /*_________________\|||/____________________________*/
    /*__________________\|/_____________________________*/
    /*___________________'______________________________*/

    addPaths(CustomPaths_ptr(new CtControlLayer("ct-control")));
    addPaths(CustomPaths_ptr(new CtPaintingLayer("ct-painting")));

    /*___________________.______________________________*/
    /*__________________/|\_____________________________*/
    /*_________________/|||\____________________________*/
    /*__________________|||_____________________________*/
    /*__________________|||_____________________________*/
	
    setActivePath("empty",false);

}

void PathsController::update() {

    activePathChanged();

    ofSetColor(255);

    getActivePath()->updatePaths();
    getActivePath()->update();
	
}

void PathsController::draw() {

    Projector *p = MappingController::getInstance().getProjector(0);

    ofSetColor(255);

    for(int i = p->quadCount()-1; i >= 0; i--) {

        getActivePath()->draw(i);
    }
}

void PathsController::addPaths(CustomPaths_ptr c) {

    paths.push_back(c);
	
}

void PathsController::setActivePath(string paths_name, bool loadDataFromLastPaths) {

    for(uint i = 0; i < paths.size(); i++) {
        if(paths[i]->getName() == paths_name) {
            setActivePath(i, loadDataFromLastPaths);
            break;
        }
    }

}

void PathsController::setActivePath(int paths_id, bool loadDataFromLastPaths) {

    PathsData *d;

    if(loadDataFromLastPaths) {
        d = getActivePath()->getData();
    }

    getActivePath()->idle();
    getActivePath()->setActive(false);

    active_path = paths_id;
    if(loadDataFromLastPaths) {
        getActivePath()->setData(d);
    }

    getActivePath()->setActive(true);
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

    return paths.at(active_path);
}

CustomPaths_ptr PathsController::getPath(int index) {

    if(index < (int)paths.size())
        return paths.at(index);
    else
        return CustomPaths_ptr();

}

bool PathsController::activePathChanged() {
    for(int i = 0; i < (int)paths.size(); i++) {
        if( (paths.at(i)->isActive()) ) {
            if(active_path.get() != i){
                setActivePath(i,true);
                return true;
            }

        }
    }
    return false;
}
