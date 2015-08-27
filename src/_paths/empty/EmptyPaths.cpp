#include "EmptyPaths.h"

EmptyPaths::EmptyPaths(string name):CustomPaths(name) {}

void EmptyPaths::setup() {}

void EmptyPaths::update(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients) {}

void EmptyPaths::draw(ofx2DMappingProjector* projector, map<string, DrawingObject_ptr> &clients) {}

void EmptyPaths::idle() {}

void EmptyPaths::resume() {}
