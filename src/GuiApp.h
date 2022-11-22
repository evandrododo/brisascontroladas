#pragma once

#include "ofxImGui.h"
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "Brisas/Brisa.h"
#include "ofxOsc.h"

#define PORT_OSC 9001

class GuiApp: public ofBaseApp {
    public:
        void setup();
        void update();
        void draw();
        void mousePressed(int x, int y, int iButton) ;

        void ligaKinect();
        void desligaKinect();

        void adicionaBrisa();

        ofxImGui::Gui gui;

        vector<Brisa*> timeline0, timeline1, timeline2, timeline3;
        vector<Brisa*> brisas;
        vector<Brisa*> brisasAtivas;
        int iBrisaFocada;
        ofxKinect kinectGlobal;
        int anguloKinect;

        GLuint btnCriaVideo,btnCriaPoligono, btnCriaKinect, btnCriaImg, 
               btnCriaGif, btnCriaMatriz, btnCriaSombras, btnCriaMandala, btnCriaBorda;

        vector<ImVec4> coresPaleta;
        ofBlendMode blendMode;
        int iBlend;
        bool desenhaMiniaturas;
        ofxOscReceiver receiverOSC;
};
