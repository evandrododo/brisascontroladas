#include "Brisa.h"

MatrizBrisa::MatrizBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, vector<ImVec4> paletaGeral, ofxOscReceiver *receiver) {
    setup();
    // Configura a brisa e defini o ícone
    brisasAtivas = brisasParent;
    receiverOSC = receiver;
    camera = 1; // 0 = camera RGB (default) | 1 = camera Depth

    desenhaBlur = true;
    brightnessGray = 0.5;
    contrastGray = 0.95;
    intervaloX = 50;

    fonteKinect = new FonteKinect(kinectGlobal, 2);
    fonteKinect->setBlur(20);

    coresPaleta = paletaGeral;
    intercalaPontos = true;
    tipoMatriz = 2;
}

void MatrizBrisa::update( float dt ) {
    fonteKinect->update(dt);
    fboBrisa.begin();

    if (clearFrames) {
        ofClear(255,255,255, 0);
    }
    intervaloY = (int)intervaloX*0.92;

    fonteKinect->pixelsBrisa.setImageType(OF_IMAGE_GRAYSCALE);
    grayImage.setFromPixels(fonteKinect->pixelsBrisa);

    grayPixels = grayImage.getPixels();
    int grayWidth = grayImage.getWidth();
    int grayHeight = grayImage.getHeight();
    int brilho = 100;
    int index = 0;
    int y = 0;
    int x = 0;
    for (y = 0; y < grayHeight; y += intervaloY) {
        for (x = 0; x < grayWidth; x += intervaloX) {
            index = y*grayWidth + x; // Pega brilho do pixel[x,y]
            brilho = grayPixels[index];
            desenhaPonto(brilho, grayWidth, grayHeight, x, y, intervaloX, intervaloY);
        }
        
        if(intercalaPontos) {
            for (x = intervaloX/2; x < grayWidth; x += intervaloX) {
                index = y*grayWidth + x; // Pega brilho do pixel[x,y]
                brilho = grayPixels[index];
                desenhaPonto(brilho, grayWidth, grayHeight, x, y+intervaloY/2, intervaloX, intervaloY);
            }
            //Coluna extra
            brilho = (grayPixels[index]  + grayPixels[index-1] + grayPixels[index-2])/3;
            desenhaPonto(brilho, grayWidth, grayHeight, x, y+intervaloY/2, intervaloX, intervaloY);
        }
    }
    // Coloca linhas extras
    int ultimoY = y - intervaloY;
    for (; y < (grayHeight+(intervaloY*18)); y += intervaloY) {
        for (x = 0; x < grayWidth; x += intervaloX) {
            index = ultimoY*grayWidth + x; // Pega brilho do pixel[x,y]
            brilho = grayPixels[index] ;
            desenhaPonto(brilho, grayWidth, grayHeight, x, y, intervaloX, intervaloY);
        }
        if(intercalaPontos) {
            for (x = intervaloX/2; x < grayWidth; x += intervaloX) {
                index = ultimoY*grayWidth + x; // Pega brilho do pixel[x,y]
                brilho = grayPixels[index] ;
                desenhaPonto(brilho, grayWidth, grayHeight, x, y+intervaloY/2, intervaloX, intervaloY);
            }
            brilho = (grayPixels[index]  + grayPixels[index-1] + grayPixels[index-2])/3;
            desenhaPonto(brilho, grayWidth, grayHeight, x, y+intervaloY/2, intervaloX, intervaloY);
        }
    }

    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void MatrizBrisa::draw() {
    aplicarShader();
}

void MatrizBrisa::desenhaPonto(int brilho, int width, int height, int x, int y, int gapX, int gapY) {
    if (tipoMatriz == 1) {
        desenhaPixels(brilho, width, height, x, y, gapX, gapY);
    } else {
        desenhaColunas(brilho, width, height, x, y, gapX, gapY);
    }
}

void MatrizBrisa::desenhaColunas(int brilho, int width, int height, int x, int y, int gapX, int gapY) {
    int altura = brilho;
    ofColor corPilar = coresPaleta[0];
    ofColor corQuadrado = coresPaleta[1];
    //corQuadrado.setBrightness(((float)x/width)*255);
    //corQuadrado.setHueAngle(((float)y/height)*360);
    ofColor corBorda = coresPaleta[2];

    ofColor corPilarEscuro = corPilar;
    corPilarEscuro.setBrightness(100); // Escurece o lado do pilar

    int tamanhoCol = gapX*0.72;

    glPushMatrix();

    //glTranslatef(x*gapX * (WIDTH / (float)width), y*gapY* (HEIGHT / (float)height) * 0.81 - altura, 0);
    glTranslatef(x, y - altura, 0);
    ofSetColor(corPilar);

    ofFill();
    ofDrawRectangle(-tamanhoCol*0.7, tamanhoCol*0.57, tamanhoCol*0.7, tamanhoCol * 5);
    ofSetColor(corBorda);
    ofNoFill();
    ofDrawRectangle(-tamanhoCol*0.7, tamanhoCol*0.57, tamanhoCol*0.7, tamanhoCol * 5);

    ofSetColor(corPilarEscuro);
    ofFill();
    ofDrawRectangle(0, tamanhoCol*0.57, tamanhoCol*0.7, tamanhoCol * 5);
    ofSetColor(corBorda);
    ofNoFill();
    ofDrawRectangle(0, tamanhoCol*0.57, tamanhoCol*0.7, tamanhoCol * 5);

    glScalef(1, 0.8, 1);
    glRotatef(45, 0, 0, 1);
    int angle = corQuadrado.getSaturation();
    corQuadrado.setSaturation(angle+altura/3);
    ofSetColor(corQuadrado);
    ofFill();
    ofDrawRectangle(0, 0, tamanhoCol, tamanhoCol);
    ofSetColor(corBorda);
    ofNoFill();
    ofDrawRectangle(0, 0, tamanhoCol, tamanhoCol);

    glPopMatrix();
}

void MatrizBrisa::desenhaPixels(int brilho, int width, int height, int x,int y, int gapX, int gapY) {
    if (brilho > 10) {
        // sorteia cor desse quadrado
        ofColor corQuad = coresPaleta[floor(ofRandom(0, coresPaleta.size()))];
        corQuad.setBrightness(brilho);
        ofSetColor(corQuad);
    }
    else {
        ofSetColor(0, 0, 0, 0);
    }
    ofDrawRectangle(x * (WIDTH / (float)width), y * (HEIGHT / (float)height), gapX* (WIDTH / (float)width), gapY*(HEIGHT / (float)height));
}

void MatrizBrisa::drawControles(int iBrisa) {

    ImGui::SliderInt("Tamanho quadrado", &intervaloX, 2, 100);

    ImGui::Checkbox("Limpa Frames", &clearFrames);
    ImGui::Checkbox("Intercala", &intercalaPontos);

    ImGui::Text("Tipo Matriz");
    ImGui::RadioButton("quadrados", &tipoMatriz, 1); ImGui::SameLine();
    ImGui::RadioButton("colunas", &tipoMatriz, 2); 

    if (ImGui::CollapsingHeader("Paleta de cores")) {
        for (int i = 0; i < coresPaleta.size(); i++) {
            string nomeCor = "Cor " + to_string(i + 1);
            ImGui::ColorEdit4(nomeCor.c_str(), (float*)&coresPaleta[i]);
            ImGui::SameLine();
            nomeCor = "[X] Excluir " + to_string(i + 1);
            if (ImGui::Button(nomeCor.c_str())) { coresPaleta.erase(coresPaleta.begin() + i); }
        }

        if (ImGui::Button(" + ")) {
            coresPaleta.push_back(ofColor::fromHex(0xC0126D));
        }
    }
    
    fonteKinect->drawControles();
    fonteKinect->fboBrisa.draw(504,504,308,231);
}
