#pragma once

#include "ofMain.h"
#include "CustomPaths.h"
#include "ofxXmlSettings.h"
#include "ofxSvg.h"

typedef tr1::shared_ptr<ofxXmlSettings> ofxXmlSettings_ptr;
typedef tr1::shared_ptr<ofxSVG> ofxSVG_ptr;
typedef tr1::shared_ptr< vector<ofPolyline> > ofPolylines_ptr;
typedef tr1::shared_ptr< vector<ofPath> > ofPaths_ptr;

namespace guardacaso {

    typedef tr1::shared_ptr<CustomPaths> CustomPaths_ptr;

    class Visuals {
		
		public:
		
            static Visuals&			get();
			void						setupMapping();
            void						setupPaths();
            void                        setupOther();
			
			void						update();
            void                        updateOutlines();
			
            void						reloadMapping(string xml_path);
            void						reloadMapping(ofxXmlSettings_ptr xml);
            void						reloadPaths(ofxXmlSettings_ptr xml);
            void						reloadOther(ofxXmlSettings_ptr xml);
            void                        reloadPathsData(CustomPaths_ptr p, ofxXmlSettings_ptr xml);
            void                        reloadSvg(string file);
            void                        importSvg(string file);
            void                        importSvg();
            void                        reloadLinesFromRaw();

            void                        saveMapping(string path, string path_svg, string path_png);
            void                        saveMappingDefault();
            void                        saveMappingAsSvg(string path);
            void                        saveMappingAsSvg();
            void                        saveOther();
            void                        savePaths();
			
            float							contentWidth();
            float							contentHeight();
            float							outputWidth();
            float							outputHeight();
            float							controlWidth();
            float							controlHeight();
            float							vidMaxWidth();
            float							vidMaxHeight();

            void						setContentWidth(float val);
            void						setContentHeight(float val);
            void						setOutputWidth(float val);
            void						setOutputHeight(float val);
            void						setControlWidth(float val);
            void						setControlHeight(float val);

            string                      galleryUrl();

            bool                        isUsingComplexQuads();
            void                        setUseComplexQuads(bool complex);

            ofParameter<bool>           controlLeft();
            void                        setControlLeft(bool setleft);
            ofPoint                     controlPoint();
            ofxSVG_ptr                  svg();
            ofPolylines_ptr             outlines();
            ofPolylines_ptr             outlinesRaw();
            ofPaths_ptr                 paths();
			
		protected:

            Visuals();
            ~Visuals();
			
        private:

            ofParameter<string>         xml_mapping, xml_other, svg_mapping, xml_paths, png_mapping;
			
			/******* WINDOW ADJUSTMENTS *****/
            ofParameter<float>			content_w, content_h;
            ofParameter<float>          output_w, output_h;
            ofParameter<float>          control_w, control_h;
            ofParameter<bool>           use_complex_quads;
            ofParameter<bool>           control_left;

            ofPoint                     control_point;

            /****** MAPPING             *****/
            ofxSVG_ptr                  _svg;
            ofPolylines_ptr             _outlines;
            ofPolylines_ptr             _outlines_raw;
            ofPaths_ptr                 _paths;
			
	};
	
}
