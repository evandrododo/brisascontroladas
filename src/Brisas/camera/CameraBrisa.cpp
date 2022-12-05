#include "CameraBrisa.h"

CameraBrisa::CameraBrisa(vector<Brisa*> *brisasParent) {
    cout << "Criando CameraBrisa" << endl;
    setup();

    cout << "Criando CameraBrisa2" << endl;
    // Configura a brisa e defini o ícone
    brisasAtivas = brisasParent;
    cout << "Criando CameraBrisa3" << endl;
    mirrorHorizontal = mirrorVertical = false;
    ofSetBackgroundAuto(false);
}


void CameraBrisa::update( float dt ) {
    cout << "Atualizando CameraBrisa" << endl;
    fboBrisa.begin();
    ofClear(255,255,255, 0);
    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void CameraBrisa::draw() {
    // aplicarShader();
}

void CameraBrisa::drawControles(int iBrisa) {
    // desenharControlesShader();
}
