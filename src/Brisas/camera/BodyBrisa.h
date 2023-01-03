#ifndef BODYBRISA_H__
#define BODYBRISA_H__

#include "../Brisa.h"
#include "ofxMovenet.h"

class BodyBrisa : public Brisa
{

public:
    BodyBrisa(vector<Brisa *> *brisasParent, vector<ImVec4> *coresPaleta);
    void draw();
    void update(float dt);

    void drawControles(int iBrisa);

    ofVideoGrabber cam;
    ofxCvColorImage colorImg;

    // config ofxtensorflow2
    // neural net input size
    std::size_t nnWidth = 512;
    std::size_t nnHeight = 288;

    // model
    ofxMovenet movenet;

    ofPixels pixelsCam;
    
    int indexDevice;
    bool mirrorHorizontal, mirrorVertical;
};

#endif