#pragma once

#include "ofMain.h"
#include "Stuff.h"

class SVGSaver : public ofThread {

public:

    void save(string path, ofPolyline line, float width, float height, ofColor strokecolor = ofColor(0)) {
        this->path = path;
        this->line = line;
        this->width = width;
        this->height = height;
        this->strokecolor = strokecolor;
        startThread();
    }

    // the thread function
    void threadedFunction() {

        ofxXmlSettings xml;

        xml.clear();

        xml.addTag("svg");
        xml.addAttribute("svg","id","svg2",0);
        xml.addAttribute("svg","xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#",0);
        xml.addAttribute("svg","xmlns","http://www.w3.org/2000/svg",0);
        xml.addAttribute("svg","width",width,0);
        xml.addAttribute("svg","height",height,0);
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

                xml.addTag("path");
                stringstream id_sstr;
                id_sstr << "path" << i;
                xml.addAttribute("path", "id", id_sstr.str(), i);
                stringstream path_sstr;
                path_sstr << "m";
                ofPoint last_p;
                for(uint k = 0; k < line.size(); k++) {
                    ofPoint cur_p = ofPoint(line[k].x, line[k].y);
                    if(k == 0)
                        path_sstr << cur_p.x << "," << cur_p.y;
                    if(k > 0)
                        path_sstr << "," << cur_p.x-last_p.x << "," << cur_p.y-last_p.y;
                    last_p = cur_p;
                }
                //path_sstr << "z";

                xml.addAttribute("path", "d", path_sstr.str(), i);
                xml.addAttribute("path", "fill", "none", i);
                xml.addAttribute("path", "stroke", Stuff::getColorAsHex(strokecolor), i);

            xml.popTag();

        xml.popTag();

        cout << "saving svg to " << path << endl;

        xml.saveFile(path);
    }

private:

    string path;
    ofPolyline line;
    float width;
    float height;
    ofColor strokecolor;

};
