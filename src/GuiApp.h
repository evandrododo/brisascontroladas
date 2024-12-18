#pragma once

#include "ofxImGui.h"
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"
#include "Brisas/Brisa.h"
#include "camera/CameraBrisa.h"
#include "camera/FinderBrisa.h"
// #include "camera/BodyBrisa.h"
#include "kinect/KinectCamBrisa.h"
#include "kinect/KinectContornosBrisa.h"
#include "kinect/SombraBrisa.h"
#include "kinect/MatrizBrisa.h"

#include "Brisas/FonteGlobalKinect.h"
#include "kinect/Colunas.h"

class GuiApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int iButton);

    void ligaKinect();
    void desligaKinect();

    void adicionaBrisa();
    void windowResized(int w, int h);
    void UIControlesGerais();
    bool isRecording = false;
    void startRecording();
    void stopRecording();
    void setFrameCountRecorded(int frameCount);
    int framesRecorded = 0;

    ofxImGui::Gui gui;

    vector<Brisa *> timeline0, timeline1, timeline2, timeline3;
    vector<Brisa *> brisas;
    vector<Brisa *> brisasAtivas;
    int iBrisaFocada;
    ofxKinect kinectGlobal;

    // Kinect
    int anguloKinect;
    int depthClippingFar, depthClippingNear;

    GLuint btnCriaVideo,
        btnCriaPoligono,
        btnCriaKinect,
        btnCriaImg,
        btnCriaFinder,
        btnCriaGif,
        btnCriaMatriz,
        btnCriaSombras,
        btnCriaMandala,
        btnCriaBorda,
        btnCriaCamera,
        btnCriaKinectCam,
        btnCriaBody,
        btnCriaColunas;

    vector<ImVec4> coresPaleta;
    ofBlendMode blendMode;
    int iBlend;
    bool desenhaMiniaturas;
};
