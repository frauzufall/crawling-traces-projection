#include "MappingTab.h"
#include "MappingController.h"
#include "Visuals.h"

using namespace guardacaso;

MappingTab::MappingTab():CustomTab() {

    control_rect.position = this->getPosition();
    control_rect.setWidth(this->getWidth());
    control_rect.setHeight(this->getHeight());

    mapping_space_rel = 0.8;

    zoom = 1;
    zoom_pos = ofPoint(0.5,0.5);

    save_btn.addListener(&Visuals::get(), &Visuals::saveMappingDefault);
//    import_svg_btn.addListener(this, &MappingTab::importSvg);
    add_obstacle_btn.addListener(this, &MappingTab::addObstacle);
    add_canvas_btn.addListener(this, &MappingTab::addCanvas);
    add_image_btn.addListener(this, &MappingTab::addImage);
    save_btn.setup("SAVE");
    add_obstacle_btn.setup("New obstacle");
    add_canvas_btn.setup("New canvas");
    add_image_btn.setup("Add URL");

    setup("MAPPING");

    setBorderColor(ofColor::black);
    setHeaderBackgroundColor(ofColor::fromHex(0xB3E3FF));
    form_mapping.setHeaderBackgroundColor(ofColor::fromHex(0x219EE8));
    mapping_list.setHeaderBackgroundColor(ofColor::fromHex(0x219EE8));
    form_mapping.setBorderColor(ofColor(0));
    mapping_list.setBorderColor(ofColor(0));

    add(&save_btn);
    add(&import_svg_btn);
    add(MappingController::getInstance().is_cal.set(
            "Calibrate",
            MappingController::getInstance().getCalibrating()));
    add(MappingController::getInstance().getCalBorder().set(
            "Border",
            MappingController::getInstance().getCalBorder(),
            0,1));
    add(MappingController::getInstance().getCalGrey().set(
            "Lightness",
            MappingController::getInstance().getCalGrey(),
            0, 255));
    add(Visuals::get().controlLeft().set(
            "Control left/right",
            Visuals::get().controlLeft()));

    add(&add_obstacle_btn);
    add(&add_canvas_btn);
    add(&add_image_btn);

    setGui();

    form_mapping.setSize(500, 300);

//    ofRegisterMouseEvents(this);

}

void MappingTab::setGui() {

    mapping_list.clear();

    for(uint i = 0; i < MappingController::getInstance().getProjector(0)->quadCount(); i++) {
        MappingQuad_ptr mq = MappingController::getInstance().getProjector(0)->getQuad(i);
        if(mq) {
            mapping_list.add(mq->nature, mq->editable, true);
        }
    }

}

void MappingTab::update() {

    if(control_rect.position != this->getPosition()) {
        control_rect.position = this->getPosition();
        control_rect.setWidth(this->getWidth());
        control_rect.setHeight(this->getHeight());

        mapping_list.setPosition(this->getPosition().x+this->getWidth()+10, this->getPosition().y);
        form_mapping.setPosition(this->getPosition().x+this->getWidth()+mapping_list.getWidth()+20, this->getPosition().y);
        form_mapping.setMappingRects();
        form_mapping.updateForms(0);
    }

    if(!ofGetMousePressed() && mapping_list.catchedElement()) {
        ofMouseEventArgs args;
        args.x = ofGetMouseX();
        args.y = ofGetMouseY();
        mouseReleased(args);
    }

    form_mapping.update();

}


void MappingTab::draw(ofPoint pos) {
    
    if(visible) {
    
        CustomTab::draw(pos);
    
        ofSetColor(200);
        ofNoFill();
        ofSetLineWidth(0.5);
    
        form_mapping.draw(pos, 0);

        mapping_list.draw();
                
    }

}

void MappingTab::updateQuadList() {

    setGui();

}

bool MappingTab::mouseMoved(ofMouseEventArgs &args) {
    CustomTab::mouseMoved(args);
    return true;
}

bool MappingTab::mouseDragged(ofMouseEventArgs &args) {

    ofPoint mouse(args.x,args.y);

    mapping_list.mouseDragged(mouse.x,mouse.y,this,&MappingTab::reorderForm);

    CustomTab::mouseDragged(args);

    return true;

}

bool MappingTab::mousePressed(ofMouseEventArgs& args) {

    ofPoint mouse(args.x,args.y);

    mapping_list.mousePressed(mouse.x,mouse.y);

    CustomTab::mousePressed(args);

    return true;

}

bool MappingTab::mouseReleased(ofMouseEventArgs &args) {

    ofPoint mouse(args.x,args.y);

    mapping_list.mouseReleased(mouse.x,mouse.y,this,&MappingTab::removeForm);

    CustomTab::mouseReleased(args);

    return true;

}

void MappingTab::importSvg() {
    Visuals::get().importSvg();
    setGui();
    form_mapping.updateForms(0);
}

void MappingTab::addCanvas() {
    MappingController::getInstance().getProjector(0)->addCanvas();
    setGui();
    form_mapping.updateForms(0);
}

void MappingTab::addObstacle() {
    MappingController::getInstance().getProjector(0)->addObstacle();
    setGui();
    form_mapping.updateForms(0);
}

void MappingTab::addImage() {
    MappingController::getInstance().getProjector(0)->addImage("images/url.png");
    setGui();
    form_mapping.updateForms(0);
}

void MappingTab::removeForm(int index) {
    MappingController::getInstance().getProjector(0)->removeQuad(index);
    Visuals::get().updateOutlines();
    setGui();
    form_mapping.updateForms(0);
}

void MappingTab::reorderForm(int old_index, int new_index) {
    if(old_index != new_index) {
        if(old_index < new_index) {
            for(int i = old_index; i < new_index; i++) {
                MappingController::getInstance().getProjector(0)->swapQuads(i, i+1);
            }
        }
        else {
            for(int i = old_index; i > new_index; i--) {
                MappingController::getInstance().getProjector(0)->swapQuads(i, i-1);
            }
        }
        form_mapping.updateForms(0);

        setGui();
    }
}

void MappingTab::reload() {
    setGui();
    form_mapping.updateForms(0);
}
