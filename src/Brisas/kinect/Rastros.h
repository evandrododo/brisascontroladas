#include "../Brisa.h"

class Rastros : public Brisa
{
public:
  Rastros();
  void draw();
  void update(float dt);

  void drawControles(int iBrisa);

  vector<ImVec4> cores;
  cv::Mat blendedFrameMat; // Frame com o rastro
  ofPixels kinectDepthPixels;
  ofImage blendedImage;
  ofFbo rastrosFbo;
};