#include "PathsData.h"
#include "Visuals.h"
#include "ObjectController.h"

using namespace guardacaso;

PathsData::PathsData() {
    color.set(ofColor::fromHex(0x2da1e3));
    sl1 = 0.5;
    sl2 = 0.5;
    sl3 = 0.5;
    rsl1 = 0.5;
    rsl2 = 0.5;
    rsl3 = 0.5;
    btn1_1 = false;
    btn1_2 = false;
    btn2_1 = false;
    btn2_2 = false;
    btn3_1 = false;
    btn3_2 = false;
    outlines = Visuals::get().outlines();
    outlines_raw = Visuals::get().outlinesRaw();
    svg = Visuals::get().svg();
}

void PathsData::copyFrom(PathsData *d) {
    color = d->color;
    sl1 = d->sl1;
    sl2 = d->sl2;
    sl3 = d->sl3;
    rsl1 = d->rsl1;
    rsl2 = d->rsl2;
    rsl3 = d->rsl3;
    btn1_1 = d->btn1_1;
    btn1_2 = d->btn1_2;
    btn2_1 = d->btn2_1;
    btn2_2 = d->btn2_2;
    btn3_1 = d->btn3_1;
    btn3_2 = d->btn3_2;
    outlines = d->outlines;
    outlines_raw = d->outlines_raw;
    svg = d->svg;
}

map<string,DrawingObject_ptr> PathsData::getDrawingObjects() {
    return ObjectController::getInstance().getClients();
}

float PathsData::getPulseTime() {
    return ObjectController::getInstance().getMaxPulseTime()*ObjectController::getInstance().getPulseTime();
}

PathsData::~PathsData(){}
