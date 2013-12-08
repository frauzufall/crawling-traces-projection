#include "SortableList.h"
#include "MappingTab.h"

using namespace guardacaso;

SortableList::SortableList():ofxPanel() {

    setup("MAPPING FORM LIST");

    padding = 10;
    margin = 1;

    clear();
    dead.clear();

}

void SortableList::clear() {
    catched_el = false;
    moving_el = -1;
    ofxPanel::clear();
    list.clear();
}

void SortableList::add(string title, ofParameter<bool> &parameter, bool at_end = true) {

    //create and insert new element in list

    ofxToggle_ptr btn = ofxToggle_ptr(new ofxToggle(parameter));
    btn->setName(title);
    ofxPanel::add(btn.get());
    int index = 0;
    if(at_end) {
        list.push_back(btn);
        index = list.size()-1;
    }
    else {
        list.insert(list.begin(),btn);
    }

    //set position of new element
    ofPoint pos = ofxPanel::getPosition();
    pos.y += 30;
    if(index > 0) {
        pos.y = list.at(index-1)->getPosition().y+list.at(index-1)->getHeight()+margin;
    }

    btn->setPosition(pos);

    //move existing elements below new element
    if(index != list.size()-1) {
        shift(index+1, btn->getHeight());
    }

    this->setSize(this->getWidth(), list.at(list.size()-1)->getPosition().y-this->getPosition().y+list.at(list.size()-1)->getHeight()+10);
}

void SortableList::add(ofxToggle_ptr btn) {
    ofxPanel::add(btn.get());
}

void SortableList::shift(int start_index, int amount) {
    for(int i = start_index; i < list.size(); i++) {
        ofxToggle_ptr btn = list.at(i);
        btn->setPosition(btn->getPosition().x, btn->getPosition().y+amount+margin);
    }
}

void SortableList::mousePressed(int x, int y) {

    for(int i = 0; i < list.size(); i++) {
        if(list.at(i)->getShape().inside(x,y)) {
            // mouse pressed on element
            moving_el = i;
            catched_el = true;
            return;
        }
    }
}
 bool SortableList::catchedElement() {
     return catched_el;
 }
