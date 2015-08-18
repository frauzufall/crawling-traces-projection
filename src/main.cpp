#include "app.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main(int argc, char *argv[]){

    ofAppGLFWWindow window;

    int w = 1600;
    int h = 900;

    window.setMultiDisplayFullscreen(true);

    ofSetupOpenGL(&window, w,h, OF_WINDOW);

    AppStart* app = new AppStart();

    app->arguments = vector<string>(argv, argv + argc);

    ofRunApp(app);

}
