#include "EmptyPaths.h"

EmptyPaths::EmptyPaths(string name):CustomPaths(name) {}

void EmptyPaths::setup() {}

void EmptyPaths::update(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients) {}

void EmptyPaths::draw(ofPtr<ofx2DMappingProjector> projector, vector<DrawingObject_ptr> &clients) {}

void EmptyPaths::idle() {}

void EmptyPaths::resume() {}
