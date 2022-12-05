#ifndef CAMBRISA_H__
#define CAMBRISA_H__

#include "../Brisa.h"

class CameraBrisa : public Brisa {

    public:
    CameraBrisa(vector<Brisa*> *brisasParent);
    void draw();
    void update( float dt );

    void drawControles(int iBrisa);

    ofVideoGrabber cam;
    int indexDevice;
    bool mirrorHorizontal, mirrorVertical;
};

#endif