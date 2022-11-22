#ifndef BRISA_H__
#define BRISA_H__

const float WIDTH = 1024;
const float HEIGHT = 768;

#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"


class Brisa {

    public:

        vector<Brisa*> *brisasAtivas;

        string iconPath;
        GLuint btnConfigBrisa; // Botão pra mostrar as configs
        ofFbo fboBrisa;
        GLuint pixelsBtn;
        ofPixels pixelsBrisa;
        void mostraBrisas();
        ofImage imgBtn;

        ofShader shaderBrisa;
        string fragShaderPath;
        float p1Shader, p2Shader, p3Shader;
        float minp1, maxp1, minp2, maxp2, minp3, maxp3;
        int iBrisaShader;
        bool ligaShader;
        bool clearFrames;

        virtual void setup();        	//Configura brisa
        virtual void update( float dt);   	//Recalcula brisa
        virtual void draw();

        virtual void drawControles(int iBrisa);
        void excluiBrisa(int iBrisa);
        virtual void desenhaMiniatura(int i, bool focada);
        void desenhaJanela(int i);

        void listaShaders();
        void loadShader(string shader);
        void listaBrisas();
        void aplicarShader();
        void desenharControlesShader();

        float brilhoBrisa, contrasteBrisa;
        bool rotacionaSozinho, torceSozinho;
        float proporcao, rotacao;
        int deslocX, deslocY;
        int opacidade;
        void desenharControlesDistorcao();

        void trazerFrente( int iBrisa ), esconderTras( int iBrisa );

        ofxOscReceiver *receiverOSC;
};

class FonteKinect : public Brisa {

    ofxKinect *kinect;
    int camera;
    float angKinect;
    float nivelFade;
    public:
    FonteKinect(ofxKinect *kinectGlobal, int cameraSelecionada);
    void setBlur( int novoBlur );
    void draw();
    void update( float dt );

    void ligaKinect();
    void desligaKinect();
    int iBlurH,iBlur,iErode,iDilate;
    int iRastro;

    void drawControles();

    bool mirrorHorizontal, mirrorVertical;
    ofxCvGrayscaleImage grayImage;
    ofVideoPlayer danceRGB, danceDepth;
    ofxCvColorImage colorImage;
    ofxCvFloatImage floatImage, floatColor;
};

class KinectBrisa : public Brisa {

    float nivelFade;
    public:
    KinectBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, ofxOscReceiver *receiver);
    void draw();
    void update( float dt );

    void desenhaMiniatura(int i);

    void drawControles(int iBrisa);

    bool mirrorHorizontal, mirrorVertical;
    bool ligaContornos;
    bool bFindHoles;
    int minArea,maxArea,blobsConsiderados;
    ofxCvContourFinder contourFinder;
    ofxCvGrayscaleImage grayImage;

    FonteKinect* fonteKinect;
};

class SombraBrisa : public Brisa {

    ofxKinect *kinecto;
    int camera;

    public:
    SombraBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, vector<ImVec4> *coresPaleta, ofxOscReceiver *receiver);
    void draw();
    void update(float dt);

    void desenhaMiniatura(int i);
    void drawControles(int iBrisa);
    bool blurKinect, desenhaBlur, mirrorHorizontal, mirrorVertical;
    float brightnessGray, contrastGray;
    int iBlur;
    bool sombraHoriz, sombraVert;
    ImVec4 corBrisa, corComplementar;
    ofPixels grayPixels;

    ofxCvGrayscaleImage grayImage, blurGray;

    FonteKinect* fonteKinect;
};

class MatrizBrisa : public Brisa {

    ofxKinect *kinecto;
    int camera;

    public:
    MatrizBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, vector<ImVec4> coresPaleta, ofxOscReceiver *receiver);
    void draw();
    void update(float dt);

    void desenhaMiniatura(int i);
    void drawControles(int iBrisa);
    void desenhaPonto(int brilho, int width, int height, int x, int y, int gapX, int gapY);
    void desenhaPixels(int brilho, int width, int height, int x, int y, int gapX, int gapY);
    void desenhaColunas(int brilho, int width, int height, int x, int y, int gapX, int gapY);
    bool blurKinect, desenhaBlur, mirrorHorizontal, mirrorVertical;
    float brightnessGray, contrastGray;
    ofPixels grayPixels;
    int intervaloX, intervaloY;
    bool intercalaPontos;
    int tipoMatriz;

    vector<ImVec4> coresPaleta;

    ofxCvGrayscaleImage grayImage, blurGray;
    FonteKinect* fonteKinect;
};

class VideoBrisa : public Brisa {
    float heightDraw, widthDraw, heightOrig, widthOrig;

    public:
    VideoBrisa(vector<Brisa*> *brisasParent, ofxOscReceiver *receiver);
    void draw();
    void update(float dt);

    ofVideoPlayer video;
    string caminhoVideo;
    ofFbo fboKinect;

    float posVideo;

    void drawControles(int iBrisa);

    void listaVideos();
    void setupVideo(string videoPath);
};

class GifBrisa : public Brisa {
    float heightDraw, widthDraw, heightOrig, widthOrig;


    public:
    GifBrisa(vector<Brisa*> *brisasParent, ofxOscReceiver *receiver);
    void draw();
    void update(float dt);

    ofVideoPlayer video;
    string caminhoVideo;
    float proporcao, rotacao;
    int deslocX, deslocY;

    void drawControles(int iBrisa);

    void listaGifs();
    void setupVideo(string videoPath);
};

class ImagemBrisa : public Brisa {

    public:
        ImagemBrisa(vector<Brisa*> *brisasParent, ofxOscReceiver *receiver);
        void draw();
        void update(float dt);

        ofImage img;
        string caminhoImagem;

        void drawControles(int iBrisa);

        void listaImagens();
        void setupImagem(string imgPath);

        bool converterGray;
        ofxCvGrayscaleImage imagemGray;

};

class PoligonoBrisa : public Brisa {

    public:
        PoligonoBrisa(vector<Brisa*> *brisasParent, vector<ImVec4> *coresPaleta, ofxOscReceiver *receiver);
        void draw();
        void update( float dt );

        void drawControles(int iBrisa);
        void desenhaPoligono(int radius);
        ImVec4 corBrisa, corComplementar;
        int vertices;
        int quantidade;
        int distancia;
        bool preencher;
        bool trocaVertices;
        bool rotacionar; 

};

#endif
