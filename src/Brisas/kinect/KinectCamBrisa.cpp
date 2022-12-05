#include "KinectCamBrisa.h"

KinectCamBrisa::KinectCamBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, ofxOscReceiver *receiver) {
    setup();
    // Configura a brisa e defini o ícone
    brisasAtivas = brisasParent;
    receiverOSC = receiver;
    mirrorHorizontal = mirrorVertical = false;
    clearFrames = true;
    nivelFade = 0;
    ofSetBackgroundAuto(false);
    fonteKinect = new FonteKinect(kinectGlobal, 1);
    minArea = 10;
    maxArea = (640*480)/2; // (kinect_width*kinect_height)/2
    blobsConsiderados = 10;
    bFindHoles = false;
}


void KinectCamBrisa::update( float dt ) {
    fonteKinect->update(dt);
    fboBrisa.begin();

    if (clearFrames) {
        // Define um alpha de acordo com o nivel Fade
        ofSetColor( ofFloatColor(1.0, 1.0, 1.0, 0.05*(nivelFade/100) ) );
    } else {
        ofSetColor(255,255,255,255);
    }

    fonteKinect->fboBrisa.draw(0,0);
    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void KinectCamBrisa::draw() {
    aplicarShader();
}

void KinectCamBrisa::drawControles(int iBrisa) {
    ImGui::Checkbox("Liga Fade", &clearFrames);
    if(clearFrames){
        ImGui::SliderFloat("Fade", &nivelFade, 0, 100);
    }

    ImGui::Checkbox("Encontrar buracos", &bFindHoles);
    ImGui::SliderInt("min Area", &minArea, 0, 100);
    ImGui::SliderInt("max Area", &maxArea, 100, (640*480) );
    ImGui::SliderInt("blobs", &blobsConsiderados, 0, 20);

    // draw fboBrisa to screen
    fonteKinect->drawControles();
    fonteKinect->drawMiniatura();
    desenharControlesShader();
}
