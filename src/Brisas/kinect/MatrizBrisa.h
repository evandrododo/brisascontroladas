#include "../Brisa.h"

class MatrizBrisa : public Brisa {

    ofxKinect *kinecto;
    int camera;

    public:
    MatrizBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent, vector<ImVec4> coresPaleta);
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
