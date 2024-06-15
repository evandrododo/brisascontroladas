#include "KinectContornosBrisa.h"

KinectContornosBrisa::KinectContornosBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent) {
    setup();
    // Configura a brisa e defini o ícone
    brisasAtivas = brisasParent;
    mirrorHorizontal = mirrorVertical = false;
    clearFrames = true;
    nivelFade = 20;
    ofSetBackgroundAuto(false);
    ligaContornos = true;
    fonteKinect = new FonteKinect(kinectGlobal, 2);
    minArea = 10;
    maxArea = (640*480)/2; // (kinect_width*kinect_height)/2
    blobsConsiderados = 10;
    bFindHoles = false;
}


void KinectContornosBrisa::update( float dt ) {
    fonteKinect->update(dt);
    fboBrisa.begin();

    if (clearFrames) {
        // Define um alpha de acordo com o nivel Fade
        ofSetColor( ofFloatColor(1.0, 1.0, 1.0, 0.05*(nivelFade/100) ) );
    } else {
        ofSetColor(255,255,255,255);
    }

    fonteKinect->fboBrisa.draw(0,0);
    if (ligaContornos) {
        fonteKinect->pixelsBrisa.setImageType(OF_IMAGE_GRAYSCALE);
        grayImage.setFromPixels(fonteKinect->pixelsBrisa);
        contourFinder.findContours(grayImage, minArea, maxArea, blobsConsiderados, bFindHoles);
        contourFinder.draw(0,0,WIDTH,HEIGHT);
    }

    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void KinectContornosBrisa::draw() {
    Brisa::draw();
}

void KinectContornosBrisa::drawControles(int iBrisa) {
    ImGui::Checkbox("Liga Fade", &clearFrames);
    if(clearFrames){
        ImGui::SliderFloat("Fade", &nivelFade, 0, 100);
    }

    ImGui::Checkbox("Contornos", &ligaContornos);
    ImGui::Checkbox("Encontrar buracos", &bFindHoles);
    ImGui::SliderInt("min Area", &minArea, 0, 100);
    ImGui::SliderInt("max Area", &maxArea, 100, (640*480) );
    ImGui::SliderInt("blobs", &blobsConsiderados, 0, 20);

    // draw fboBrisa to screen
    fonteKinect->drawControles();
    fonteKinect->drawMiniatura();

    Brisa::drawControles(iBrisa);
}
