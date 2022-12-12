#include "CameraBrisa.h"

// Class of Video Grabber to get input from cameras 
CameraBrisa::CameraBrisa(vector<Brisa*> *brisasParent) {
    setup();
    brisasAtivas = brisasParent;

    mirrorHorizontal = mirrorVertical = false;

    ofSetBackgroundAuto(false);
    
    // Define index como -1 (nenhum dispositivo selecionado)
    indexDevice = -1;
}


void CameraBrisa::update( float dt ) {
    fboBrisa.begin();
    ofSetColor(255,255,255,255);
    if (cam.isInitialized())
    {
        cam.update();
        cam.draw(0, 0);
    }
    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void CameraBrisa::draw() {
    aplicarShader();
}

void CameraBrisa::drawControles(int iBrisa) {
    if (ImGui::Button("Carregar Camera")) { 
        ImGui::OpenPopup("deviceIndex");
    }
    if (ImGui::BeginPopup("deviceIndex")) {
        if (ImGui::Selectable("0")) {
            indexDevice = 0;
            cam.setDeviceID(indexDevice);
            cam.setup(WIDTH, HEIGHT);
        }
        if (ImGui::Selectable("1")) {
            indexDevice = 1;
            cam.setDeviceID(indexDevice);
            cam.setup(WIDTH, HEIGHT);
        }
        if (ImGui::Selectable("2")) {
            indexDevice = 2;
            cam.setDeviceID(indexDevice);
            cam.setup(WIDTH, HEIGHT);
        }
        ImGui::EndPopup();
    } 
}
