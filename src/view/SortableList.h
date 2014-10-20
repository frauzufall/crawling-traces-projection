#pragma once
#include "ofMain.h"
#include "ofxGui.h"

typedef std::shared_ptr<ofxToggle> ofxToggle_ptr;

namespace guardacaso {

    class SortableList : public ofxPanel {

    public:
        SortableList();
        void add(string title, ofParameter<bool> &parameter, bool at_end);
        void add(ofxToggle_ptr btn);
        void clear();
        void shift(int start_index, int amount);
        bool catchedElement();

        template<class ListenerClass>
        void mouseDragged(int x, int y, ListenerClass * listener, void (ListenerClass::*draggedInside)(int, int)) {
            if(catched_el && moving_el >= 0 && moving_el < (int)list.size()) {

                //move el with drag
                list.at(moving_el)->setPosition(x, y);

                if(this->getShape().inside(x,y)) {
                    int new_pos = 0;
                    for(int i = 0; i < (int)list.size(); i++) {
                        if(list.at(i)->getPosition().y < y) {
                            if(moving_el<i) {
                                new_pos = i;
                            }
                            if(moving_el>i) {
                                new_pos = i+1;
                            }
                        }
                    }
                    if(moving_el != new_pos) {
                        ((listener)->*draggedInside)(moving_el, new_pos);
                        catched_el = true;
                        moving_el = new_pos;
                    }
                }

            }
        }

        void mousePressed(int x, int y);

        template<class ListenerClass>
        void mouseReleased(int x, int y, ListenerClass * listener, void (ListenerClass::*draggedOutside)(int)){

            if(catched_el) {

                if(!this->getShape().inside(x,y)) {
                    ((listener)->*draggedOutside)(moving_el);
                }
                else {
                    //reorder elements
                    ofPoint pos = ofxPanel::getPosition();
                    pos.y += 30;
                    if(moving_el > 0) {
                        pos.y = list.at(moving_el-1)->getPosition().y+list.at(moving_el-1)->getHeight()+margin;
                    }
                    list.at(moving_el)->setPosition(pos);
                }

            }

            catched_el = false;
            moving_el = -1;

        }

    private:
        vector<ofxToggle_ptr> list;
        int padding;
        int margin;
        bool catched_el;
        int moving_el;
        ofxPanel *parent;

    };

}
