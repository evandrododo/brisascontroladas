#ifndef KINECTCAMBRISA_H__
#define KINECTCAMBRISA_H__

#include "../Brisa.h"

class KinectCamBrisa : public Brisa {

    float nivelFade;
    public:
    KinectCamBrisa(ofxKinect *kinectGlobal, vector<Brisa*> *brisasParent);
    void draw();
    void update( float dt );

    // TODO: Remover?
    void desenhaMiniatura(int i);

    void drawControles(int iBrisa);

    bool mirrorHorizontal, mirrorVertical;
    bool bFindHoles;
    int minArea,maxArea,blobsConsiderados;
    ofxCvContourFinder contourFinder;
    ofxCvGrayscaleImage grayImage;

    FonteKinect* fonteKinect;
};

#endif