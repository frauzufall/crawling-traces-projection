#include "app.h"
#include "ofAppGLFWWindow.h"

typedef std::shared_ptr<AppStart> AppStart_ptr;

//========================================================================
int main(int argc, char *argv[]){

    ofAppGLFWWindow window;

    int w = 100;
    int h = 100;

    window.setMultiDisplayFullscreen(true);

    ofSetupOpenGL(&window, w,h, OF_WINDOW);

    AppStart_ptr app = AppStart_ptr(new AppStart());

    app->arguments = vector<string>(argv, argv + argc);

    ofRunApp(app.get());

}
