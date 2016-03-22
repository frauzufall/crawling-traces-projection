#include "app.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main(int argc, char *argv[]){

	ofGLFWWindowSettings settings;
	settings.multiMonitorFullScreen = true;
	settings.windowMode = OF_WINDOW;
	auto window = ofCreateWindow(settings);
	auto app = std::make_shared<AppStart>();
	app->arguments = vector<string>(argv, argv + argc);
	ofRunApp(window, app);
	ofRunMainLoop();

}
