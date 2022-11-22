#include "Brisa.h"

PoligonoBrisa::PoligonoBrisa(vector<Brisa*> *brisasParent, vector<ImVec4> *coresPaleta, ofxOscReceiver *receiver) {
    setup();
    // Configura a brisa e defini o ícone
    brisasAtivas = brisasParent;
    receiverOSC = receiver;

    vertices = 6;
    trocaVertices = true;
    quantidade = 10;
    distancia = 70;
    preencher = true;
    rotacionar = false;

    int sizePaleta = coresPaleta->size();
    int iCor1 = ofRandom(0, sizePaleta-1);
    int iCor2 = ofRandom(0, sizePaleta-1);
    // Caso as cores sejam iguais troca a segunda	
    if (iCor1 == iCor2) {
        iCor2 = iCor2+1 > sizePaleta ? iCor2-1 : iCor2+1;
    }
    corBrisa = coresPaleta->at(iCor1);
    corComplementar = coresPaleta->at(iCor2);
}

void PoligonoBrisa::update( float dt ) {
    float time0 = ofGetElapsedTimef();
    if( trocaVertices ) {
        vertices = int(abs(sin(time0*0.1)*4)) + 4;
    }
    fboBrisa.begin();

    ofClear(255,255,255, 0);

    bool cor1ou2 = quantidade%2;
    for (int i = quantidade; i > 0; i--) {
        cor1ou2 ^= 1;
        if( cor1ou2 ) {
            ofSetColor(corBrisa);
        } else {
            ofSetColor(corComplementar);
        }
        desenhaPoligono(abs(sin(time0*0.7-.3))*100 + i*distancia);
    }



    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void PoligonoBrisa::draw() {
    aplicarShader();
}

void PoligonoBrisa::drawControles(int iBrisa) {
    ImGui::Text("poligon");

    ImGui::ColorEdit4("Base", (float*)&corBrisa);
    ImGui::ColorEdit4("Complementar", (float*)&corComplementar);

    ImGui::SliderInt("Vértices", &vertices, 0, 9);ImGui::SameLine();
    ImGui::Checkbox("Troca sozinho", &trocaVertices);
    ImGui::SliderInt("Quantidade", &quantidade, 0, 30);
    ImGui::SliderInt("Distância", &distancia, 1, 150);

    ImGui::Checkbox("Rotacionar", &rotacionar);
    ImGui::Checkbox("Preencher", &preencher);

    ImGui::Checkbox("Ligar Shader", &ligaShader);

}


// Desenha poligono
void PoligonoBrisa::desenhaPoligono(int radius) {
    if(vertices < 3)
        vertices = 3;
    //corrigindo tamanho aparente
    radius = radius - vertices*8;
    if(preencher) {
        ofFill();
    }else {
        ofNoFill();
    }
    glPushMatrix();

    glTranslatef(WIDTH/2,HEIGHT/2, 0);


    if (rotacionar) {
        glRotatef( sin(ofGetElapsedTimef()*0.5)*50 + (360/vertices-1)-(360/vertices) + 100, 0, 0, 1); 
    } else {
        glRotatef( 180, 0, 0, 1); 
    }

    ofBeginShape();
    for(int i = 0; i < vertices+1; i++) {
        float theta = (TWO_PI/vertices) * i;
        ofVertex( sin(theta)*radius , cos(theta)*radius ,0);
    }
    ofEndShape();

    glPopMatrix();
}
