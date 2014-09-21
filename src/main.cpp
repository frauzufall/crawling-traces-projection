#include "app.h"
#include "ofAppGLFWWindow.h"

typedef std::shared_ptr<AppStart> AppStart_ptr;

//========================================================================
int main(){

    ofAppGLFWWindow window;

    int w = 100;
    int h = 100;

    window.setMultiDisplayFullscreen(true);

    ofSetupOpenGL(&window, w,h, OF_WINDOW);

    AppStart_ptr app = AppStart_ptr(new AppStart());
    ofRunApp(app.get());

}
