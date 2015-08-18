#include "EmptyPaths.h"

EmptyPaths::EmptyPaths(string name):CustomPaths(name) {}

void EmptyPaths::setup() {}

void EmptyPaths::update(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients) {}

void EmptyPaths::draw(ofPolylines_ptr lines, map<string, DrawingObject_ptr> &clients) {}

void EmptyPaths::idle() {}

void EmptyPaths::resume() {}
