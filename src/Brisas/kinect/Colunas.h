#include "../Brisa.h"

class Colunas : public Brisa
{

public:
  Colunas();
  void draw();
  void update(float dt);

  void drawControles(int iBrisa);

  vector<ImVec4> cores;
};