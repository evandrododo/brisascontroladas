#include "Colunas.h"
#include "../FonteGlobalKinect.h"

Colunas::Colunas()
{
  Brisa::setup();
  // purple
  cores.push_back(ImVec4(0.4, 0.2, 0.8, 1.0));
  // pink
  cores.push_back(ImVec4(0.9, 0.2, 1.0, 1.0));
  // black
  cores.push_back(ImVec4(0.0, 0.0, 0.0, 1.0));
}

void Colunas::update(float dt)
{
  Brisa::update(dt);

  fboBrisa.begin();
  ofClear(255, 255, 255, 0);
  ofSetColor(255, 255, 255, 255);
  FonteGlobalKinect::getInstance().getRgbTexture().draw(0, 0, fboBrisa.getWidth(), fboBrisa.getHeight());
  fboBrisa.end();
  fboBrisa.readToPixels(pixelsBrisa);
}

void Colunas::draw()
{
  Brisa::draw();
}

void Colunas::drawControles(int iBrisa)
{
  if (ImGui::CollapsingHeader("Cores"))
  {
    ImGui::ColorEdit4("Cor 1", (float *)&cores[0]);
    ImGui::ColorEdit4("Cor 2", (float *)&cores[1]);
    ImGui::ColorEdit4("Cor 3", (float *)&cores[2]);
  }
}