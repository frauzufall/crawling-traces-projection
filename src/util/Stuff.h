#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxXmlSettings.h"

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

typedef std::shared_ptr<ofxSVG> ofxSVG_ptr;


class Stuff {

public:

    static vector<ofPoint> getPointsBetween(ofPoint p1, ofPoint p2, int count) {

        vector<ofPoint> res;
        res.clear();

        if(count > 0) {

            //float distance = p1.distance(p2);
            float a = -1/((float)(2*count));
            for(float i = 0; i < count; i++) {
                res.push_back(p1+(p2-p1)*(i/count+ofRandom(-a,a)));
            }
        }

        return res;

    }

    static string getHexCode(unsigned char c) {

       stringstream ss;
       ss << uppercase << setw(2) << setfill('0') << std::hex;
       ss << +c;

       return ss.str();
    }

    static string getColorAsHex(ofColor c) {
        return getColorAsHex(c.r,c.g,c.b);
    }

    static string getColorAsHex(int r, int g, int b) {
        stringstream ss;
        ss << "#" << getHexCode(r) << getHexCode(g) << getHexCode(b);
        return ss.str();
    }

    static string secondsToString(int time) {
        stringstream res;
        int numdays = floor((time % 31536000) / 86400);
        if(numdays>0)
            res << numdays << "d ";
        int numhours = floor(((time % 31536000) % 86400) / 3600);
        if(numhours>0)
            res << numhours << "h ";
        int numminutes = floor((((time % 31536000) % 86400) % 3600) / 60);
        if(numminutes>0)
            res << numminutes << "min ";
        int numseconds = (((time % 31536000) % 86400) % 3600) % 60;
        if(numseconds>0)
            res << numseconds << "sec";
        return res.str();
   }

    static ofPolyline ofPathToOfPolyline(ofPath path, bool straight_lines) {

        ofPolyline polyline;

        vector<ofPath::Command> commands;

        vector<ofPath::Command>& pathCommands = path.getCommands();
        for (uint j=0; j<pathCommands.size(); j++) {
            commands.push_back(pathCommands[j]);
        }

        polyline.clear();
            int curveResolution = ofGetStyle().curveResolution;
            //maybe a better way?
            ofPath tempPath;
            int arcResolution = tempPath.getCircleResolution();

        for(uint i=0; i<commands.size(); i++) {
            switch(commands[i].type) {
                case ofPath::Command::lineTo:
                    polyline.addVertex(commands[i].to);
                    break;
                case ofPath::Command::curveTo:
                    if(straight_lines)
                        polyline.addVertex(commands[i].to);
                    else
                        polyline.curveTo(commands[i].to, curveResolution);
                    break;
                case ofPath::Command::bezierTo:
                    if(straight_lines)
                        polyline.addVertex(commands[i].to);
                    else
                        polyline.bezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
                    break;
                case ofPath::Command::quadBezierTo:
                    if(straight_lines)
                        polyline.addVertex(commands[i].to);
                    else
                        polyline.quadBezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
                    break;
                case ofPath::Command::arc:
                    if(straight_lines)
                        polyline.addVertex(commands[i].to);
                    else
                        polyline.arc(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, arcResolution);
                    break;
                case ofPath::Command::arcNegative:
                    if(straight_lines)
                        polyline.addVertex(commands[i].to);
                    else
                        polyline.arcNegative(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, arcResolution);
                    break;
                case ofPath::Command::moveTo:
                    //?
                    break;
                default: break;
            }
        }

        return polyline;

    }

    static bool hasEnding (std::string const &fullString, std::string const &ending) {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

    static void saveLineAsSvg(string path, ofPolyline line, float width, float height, ofColor strokecolor = ofColor(0)) {

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
                xml.addAttribute("path", "stroke", getColorAsHex(strokecolor), i);

            xml.popTag();

        xml.popTag();

        cout << "saving svg to " << path << endl;

        xml.saveFile(path);
    }

    static std::string exec(const char* cmd) {
        FILE* pipe = popen(cmd, "r");
        if (!pipe) return "ERROR";
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        return result;
    }

    // trim from start
    static inline std::string &ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s) {
            return ltrim(rtrim(s));
    }
   /*
    static int getDir (string dir, vector<string> &files) {
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(dir.c_str())) == NULL) {
            cout << "Error(" << errno << ") opening " << dir << endl;
            return errno;
        }

        while ((dirp = readdir(dp)) != NULL) {
            files.push_back(string(dirp->d_name));
        }
        closedir(dp);
        return 0;
    }



    static void ProcessDirectory(std::string directory) {
        std::string dirToOpen = path + directory;
        auto dir = opendir(dirToOpen.c_str());

        //set the new path for the content of the directory
        path = dirToOpen + "/";

        std::cout << "Process directory: " << dirToOpen.c_str() << std::endl;

        if(NULL == dir) {
            std::cout << "could not open directory: " << dirToOpen.c_str() << std::endl;
            return;
        }

        auto entity = readdir(dir);

        while(entity != NULL) {
            ProcessEntity(entity);
            entity = readdir(dir);
        }

        //we finished with the directory so remove it from the path
        path.resize(path.length() - 1 - directory.length());
        closedir(dir);
    }

    static void ProcessEntity(struct dirent* entity) {
        //find entity type
        if(entity->d_type == DT_DIR) {
            //it's an direcotry
            //don't process the  '..' and the '.' directories
            if(entity->d_name[0] == '.') {
                return;
            }

            //it's an directory so process it
            ProcessDirectory(std::string(entity->d_name));
            return;
        }

        if(entity->d_type == DT_REG) {
            //regular file
            ProcessFile(std::string(entity->d_name));
            return;
        }

        //there are some other types
        //read here http://linux.die.net/man/3/readdir
        std::cout << "Not a file or directory: " << entity->d_name << std::endl;
    }

    void ProcessFile(std::string file) {
        std::cout << "Process file     : " << fileToOpen.c_str() << std::endl;
        //if you want to do something with the file add your code here
    }


*/

};


