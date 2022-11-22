#include "Brisa.h"

SombraBrisa::SombraBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, vector<ImVec4> *coresPaleta, ofxOscReceiver *receiver) {
    setup();
    brisasAtivas = brisasParent;
    receiverOSC = receiver;

    mirrorHorizontal = mirrorVertical = false;
    desenhaBlur = true;
    blurKinect = false;
    brightnessGray = 0.5;
    contrastGray = 0.95;
    iBlur = 2;
    sombraHoriz = true;
    sombraVert = false;

    int sizePaleta = coresPaleta->size();
    int iCor1 = ofRandom(0, sizePaleta-1);
    int iCor2 = ofRandom(0, sizePaleta-1);

    // Caso as cores sejam iguais troca a segunda	
    if (iCor1 == iCor2) {
        iCor2 = iCor2 + 1 > sizePaleta ? iCor2 - 1 : iCor2 + 1;
    }
    corBrisa = coresPaleta->at(iCor1);
    corComplementar = coresPaleta->at(iCor2);

    fonteKinect = new FonteKinect(kinectGlobal, 2);
    fonteKinect->setBlur(20);
}

void SombraBrisa::update( float dt ) {
    fonteKinect->update(dt);
    fboBrisa.begin();

    if (clearFrames) {
        ofClear(255,255,255, 0);
    }

    fonteKinect->pixelsBrisa.setImageType(OF_IMAGE_GRAYSCALE);
    grayImage.setFromPixels(fonteKinect->pixelsBrisa);

    grayImage.mirror(mirrorVertical, mirrorHorizontal);
    grayImage.brightnessContrast(brightnessGray, contrastGray);

    blurGray = grayImage;
    for (int i = 0; i < iBlur; i++) {
        blurGray.erode_3x3();
        blurGray.dilate_3x3();
        blurGray.blurHeavily();
        blurGray.erode_3x3();
        blurGray.dilate_3x3();
        blurGray.blur(3);
    }
    ofSetColor(corBrisa);
    blurGray.draw(0, 0, WIDTH, HEIGHT);
    // Desenha as sombras repetidas (efeito caleidoscopio)
    if (sombraHoriz || sombraVert) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        if (sombraVert) {
            ofSetColor(corComplementar);
            blurGray.mirror(true, false);
            blurGray.draw(0, 0, WIDTH, HEIGHT);
        }
        if (sombraHoriz) {
            ofSetColor(corComplementar);
            blurGray.mirror(false, true);
            blurGray.draw(0, 0, WIDTH, HEIGHT);
        }
        if (sombraVert && sombraHoriz) {
            ofSetColor(corBrisa);
            blurGray.mirror(true, false);
            blurGray.draw(0, 0, WIDTH, HEIGHT);
        }
    }

    grayPixels = grayImage.getPixels();

    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void SombraBrisa::draw() {
    aplicarShader();
}

void SombraBrisa::drawControles(int iBrisa) {
    ImGui::ColorEdit4("Base", (float*)&corBrisa);
    ImGui::ColorEdit4("Complementar", (float*)&corComplementar);

    // Camêra fonte
    ImGui::Text("Sombras");
    ImGui::Checkbox("--", &sombraHoriz);
    ImGui::Checkbox("|", &sombraVert);

    ImGui::Checkbox("Limpa Frames", &clearFrames);

    fonteKinect->drawControles();

    fonteKinect->fboBrisa.draw(504,504,308,231);
}
