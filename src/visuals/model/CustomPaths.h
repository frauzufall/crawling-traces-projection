#pragma once

#include "PathsData.h"

namespace guardacaso {

    class CustomPaths {
	
	public:
		
        CustomPaths(string title);
		
        void updatePaths();
		
		virtual void setup() = 0;
		
		virtual void update() = 0;

        virtual void draw(int path) = 0;

        virtual void idle() = 0;
        virtual void resume() = 0;
		
		string getName();

        bool isSetupDone();
        bool isLoaded();
        void setLoaded();

        ofParameter<bool> isActive();
        void setActive(bool active);

        PathsData* getData();
        void setData(PathsData * d);

        void setColor(ofColor val){data.color = val;}
        void setSlider1_vertical(float val){data.sl1 = val;}
        void setSlider2_vertical(float val){data.sl2 = val;}
        void setSlider3_vertical(float val){data.sl3 = val;}
        void setSlider1_rotate(float val){data.rsl1 = val;}
        void setSlider2_rotate(float val){data.rsl2 = val;}
        void setSlider3_rotate(float val){data.rsl3 = val;}
        void setButton1_top(bool val){data.btn1_1 = val;}
        void setButton2_top(bool val){data.btn2_1 = val;}
        void setButton3_top(bool val){data.btn3_1 = val;}
        void setButton1_bottom(bool val){data.btn1_2 = val;}
        void setButton2_bottom(bool val){data.btn2_2 = val;}
        void setButton3_bottom(bool val){data.btn3_2 = val;}


        string  name_sl1,name_sl2,name_sl3,
                name_rsl1,name_rsl2,name_rsl3,
                name_btn1_1,name_btn1_2,name_btn2_1,name_btn2_2,name_btn3_1,name_btn3_2;
		
	protected:
		
        PathsData data;
		
		int delay;
        int lastupdate;

        ~CustomPaths(){}
		
	private:
		
        string name;

        bool setup_done;
        bool loaded;
        ofParameter<bool> active;
	
	};

}

