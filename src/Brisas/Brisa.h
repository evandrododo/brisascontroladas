#ifndef BRISA_H__
#define BRISA_H__

const float WIDTH = 1024;
const float HEIGHT = 768;

#include "ofxImGui.h"
// #include "ofxTensorFlow2.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"
#include "../WindowManager.h"

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
        void desenharControlesShader();

        float brilhoBrisa, contrasteBrisa;
        bool rotacionaSozinho;
        float proporcao, rotacao;
        int deslocX, deslocY;
        int opacidade;
        
        // Posição e tamanho da brisa
        void desenharControlesPosicao();
        int fboWidth, fboHeight;
        int fboX, fboY;

        void trazerFrente( int iBrisa ), esconderTras( int iBrisa );
};

class FonteKinect : public Brisa {

    ofxKinect *kinect;
    int camera;
    float angKinect;
    float nivelFade;
    public:
    FonteKinect(ofxKinect *kinectGlobal, int cameraSelecionada);
    void setBlur( int novoBlur );
    void draw() override;
    void update( float dt );

    void ligaKinect();
    void desligaKinect();
    int iBlurH,iBlur,iErode,iDilate;
    int iRastro;

    void drawControles();
    void drawMiniatura();

    bool mirrorHorizontal, mirrorVertical;
    ofxCvGrayscaleImage grayImage;
    ofVideoPlayer danceRGB, danceDepth;
    ofxCvColorImage colorImage;
    ofxCvFloatImage floatImage, floatColor;
};
class VideoBrisa : public Brisa {
    float heightDraw, widthDraw, heightOrig, widthOrig;

    public:
    VideoBrisa(vector<Brisa*> *brisasParent);
    void draw() override;
    void update(float dt);

    ofVideoPlayer video;
    string caminhoVideo;
    ofFbo fboKinect;

    float posVideo;

    void drawControles(int iBrisa) override;

    void listaVideos();
    void setupVideo(string videoPath);
};

class GifBrisa : public Brisa {
    float heightDraw, widthDraw, heightOrig, widthOrig;


    public:
    GifBrisa(vector<Brisa*> *brisasParent);
    void draw() override;
    void update(float dt);

    ofVideoPlayer video;
    string caminhoVideo;
    float proporcao, rotacao;
    int deslocX, deslocY;

    void drawControles(int iBrisa) override;

    void listaGifs();
    void setupVideo(string videoPath);
};

class ImagemBrisa : public Brisa {

    public:
        ImagemBrisa(vector<Brisa*> *brisasParent);
        void draw() override;
        void update(float dt);

        ofImage img;
        string caminhoImagem;

        void drawControles(int iBrisa) override;

        void listaImagens();
        void setupImagem(string imgPath);

        bool converterGray;
        ofxCvGrayscaleImage imagemGray;

};

class PoligonoBrisa : public Brisa {

    public:
        PoligonoBrisa(vector<Brisa*> *brisasParent, vector<ImVec4> *coresPaleta);
        void draw() override;
        void update( float dt );

        void drawControles(int iBrisa) override;
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
