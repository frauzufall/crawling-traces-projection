#pragma once

#include "ofxGui.h"

namespace guardacaso {

    class CustomTab : public ofxGuiPage {

    public:

        CustomTab():ofxGuiPage(){

            margin = 10;

            group_config.spacing = 7;
            group_config.headerBackgroundColor = ofColor::fromHex(0x368082);
            group_config.backgroundColor = ofColor(0,0,0,0);
            group_config.fillColor = ofColor(255,150);
            group_config.borderColor = ofColor(0,0,0,0);

            slider_config.backgroundColor = ofColor(255, 66);
            slider_config.fillColor = ofColor(255,150);
            slider_config.borderColor = ofColor(0,0,0,0);
            slider_config.precision = 2;

            toggle_config.backgroundColor = ofColor(0,0,0,0);
            toggle_config.fillColor = ofColor(255,150);
            toggle_config.borderColor = ofColor(255,66);

            toggle_auto_width_config = toggle_config;
            toggle_auto_width_config.shape.width = 0;

            label_config.backgroundColor = ofColor(0,0,0,0);
            label_config.textColor = ofColor(255);
            label_config.borderColor = ofColor(0,0,0,0);

        }

        ~CustomTab(){}

        using ofxGuiGroup::setup;

        int margin;

        ofxFloatSlider::Config slider_config;
        ofxBaseGui::Config toggle_config;
        ofxBaseGui::Config toggle_auto_width_config;
        ofxBaseGui::Config label_config;
        ofxGuiGroup::Config group_config;


    };

}
