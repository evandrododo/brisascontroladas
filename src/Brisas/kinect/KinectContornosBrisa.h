#include "../Brisa.h"

class KinectContornosBrisa : public Brisa {

    float nivelFade;
    public:
    KinectContornosBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent);
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