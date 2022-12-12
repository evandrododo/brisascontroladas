#include "../Brisa.h"

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
