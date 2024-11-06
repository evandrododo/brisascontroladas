#ifndef FONTEKINECTGLOBAL_H__
#define FONTEKINECTGLOBAL_H__

#include "ofxImGui.h"
#include "ofxKinect.h"
#include "./Brisa.h"

// Kinect Singleton
class FonteGlobalKinect : public Brisa
{
public:
  // Método estático para acessar a instância única
  static FonteGlobalKinect &getInstance()
  {
    static FonteGlobalKinect instance; // Instância é criada apenas uma vez e reutilizada
    return instance;
  }

  bool getIsConnected()
  {
    return kinect.isConnected();
  }

  void ligaKinect()
  {
    if (!kinect.isConnected())
    {
      kinect.setRegistration(true);
      kinect.init();
      kinect.open();
      kinect.setCameraTiltAngle(0);
      kinect.setDepthClipping(500, 2000); // Ajusta o corte de profundidade conforme necessário
    }
  }

  void desligaKinect()
  {
    kinect.close();
  }

  void update()
  {
    if (kinect.isConnected())
    {
      kinect.update();
      if (kinect.isFrameNew())
      {
        // Atualiza os pixels da imagem de profundidade
        depthPixels = kinect.getDepthPixels();
        depthTexture = kinect.getDepthTexture();
        rgbTexture = kinect.getTexture();
      }
    }
  }

  void setAngle(int angle)
  {
    kinect.setCameraTiltAngle(angle);
  }

  void setDepthClipping(int near, int far)
  {
    kinect.setDepthClipping(near, far);
  }

  // Obtém os pixels da imagem de profundidade
  ofPixels &getDepthPixels()
  {
    return depthPixels;
  }

  // Obtém a textura da imagem de profundidade (para desenhar)
  ofTexture &getDepthTexture()
  {
    return depthTexture;
  }

  ofTexture &getRgbTexture()
  {
    return rgbTexture;
  }

  ofxKinect kinect;
  ofPixels depthPixels;
  ofTexture depthTexture;
  ofTexture rgbTexture;

  // Deleta o construtor de cópia e o operador de atribuição
  FonteGlobalKinect(const FonteGlobalKinect &) = delete;
  FonteGlobalKinect &operator=(const FonteGlobalKinect &) = delete;

private:
  // Construtor privado para impedir a criação direta
  FonteGlobalKinect()
  {
    std::cout << "FonteGlobalKinect created!" << std::endl;
  }

  ~FonteGlobalKinect()
  {
    desligaKinect();
  }
};

#endif // FONTEKINECTGLOBAL_H__