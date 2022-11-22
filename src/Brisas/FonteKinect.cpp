#include "Brisa.h"

FonteKinect::FonteKinect(ofxKinect *kinectGlobal, int cameraSelecionada = 0) {
    setup();
    // Configura a brisa e defini o ícone
    kinect = kinectGlobal;
    camera = cameraSelecionada; // 0 = camera RGB (default) | 1 = camera Depth

    mirrorHorizontal = mirrorVertical = false;
    ofSetBackgroundAuto(false);
    iRastro = 0;
    iBlur = 4;
    iBlurH = 1;
    iErode = 10;
    iDilate = 10;
    floatColor.allocate(640,480);
    floatImage.allocate(640,480);
    grayImage.allocate(640,480);
    colorImage.allocate(640,480);

    floatColor.setROI(20,100,520,480);
    colorImage.setROI(20,100,520,480);
    grayImage.setROI(20,100,520,480);
    floatImage.setROI(20,100,520,480);
}

void FonteKinect::ligaKinect() {
    if( !kinect->isConnected() ) {
        kinect->setRegistration(true);
        kinect->init();
        kinect->open();
        kinect->setCameraTiltAngle(0);
    }
}

void FonteKinect::desligaKinect() {
    kinect->close();
}

void FonteKinect::update( float dt ) {
    fboBrisa.begin();

    ofSetColor(255,255,255,255);

    // Camera RGB
    if (camera == 1) {
        // Kinect ta funcionando
        if ( kinect->isConnected() ) {
            kinect->update();
            colorImage.setFromPixels(kinect->getPixels());
            floatColor = colorImage;
        } else {
            // Kinect desligado, carrega video Demo
            if( danceRGB.isLoaded() )  {
                danceRGB.update();
            } else {
                danceRGB.load("../data/kinectdemo/rgbdance.mp4");
                danceRGB.play();
            }
            colorImage.setFromPixels(danceRGB.getPixels());
            floatColor = colorImage;
        }
    } else {
    // Camera Depth
        // Kinect ta funcionando
        if ( kinect->isConnected() ) {
            kinect->update();
            grayImage.setFromPixels(kinect->getDepthPixels());
            floatColor = grayImage;
            
        } else {
            // Kinect desligado, carrega video Demo
            if( danceDepth.isLoaded() )  {
                danceDepth.update();
            } else {
                danceDepth.load("../data/kinectdemo/depthdance.mp4");
                danceDepth.play();
            }
            colorImage.setFromPixels(danceDepth.getPixels());
            floatColor = colorImage;
        }
    }
    
    int maxI = 0;
    maxI = iBlur > maxI ? iBlur : maxI;
    maxI = iErode > maxI ? iErode : maxI;
    maxI = iDilate > maxI ? iDilate : maxI;

    // Aplica blur, erode e dilate
    for (int i = 0; i < maxI; i++) {
        if (iErode >= i )
            floatColor.erode();
        if (iDilate >= i )
            floatColor.dilate();
        if (iBlur >= i )
            floatColor.blurGaussian(5);
        if (iBlurH >= i )
            floatColor.blurGaussian(15);
    }

    //Aplica o Rastro
    ofFloatPixels & pixF = floatImage.getFloatPixelsRef();
    ofFloatPixels & pixA = floatColor.getFloatPixelsRef();
    int numPixels = pixF.size();
    for (int i = 0; i < numPixels; i++) {
        pixF[i] = pixF[i] *((float)iRastro/100) + pixA[i] * (1 - (float)iRastro/100);// Aumenta contraste de distancia
    }
    floatImage.flagImageChanged();
    floatColor = floatImage;

    floatColor.mirror(mirrorVertical, mirrorHorizontal);
    if (camera == 1) {
        floatColor.draw(0,0,WIDTH,HEIGHT);
    } else {
        grayImage = floatColor;
        grayImage.brightnessContrast(brilhoBrisa, contrasteBrisa);
        grayImage.drawROI(0,0,WIDTH,HEIGHT);
    }

    fboBrisa.end();
    fboBrisa.readToPixels(pixelsBrisa);
}

void FonteKinect::draw() {
    aplicarShader();
}

void FonteKinect::drawControles() {
    if (ImGui::CollapsingHeader("Kinect")) {
	// Camêra fonte
	ImGui::Text("Camêra selecionada:");
	ImGui::RadioButton("RGB", &camera, 1); ImGui::SameLine();
	ImGui::RadioButton("Profundidade", &camera, 2);

	ImGui::Checkbox("Espelho Horizontal", &mirrorHorizontal);
	ImGui::Checkbox("Espelho Vertical", &mirrorVertical);

        ImGui::SliderInt("blur", &iBlur, 0, 50);
        ImGui::SliderInt("blur HEAVILY", &iBlurH, 0, 50);
        ImGui::SliderInt("erode", &iErode, 0, 50);
        ImGui::SliderInt("dilate", &iDilate, 0, 50);
        ImGui::SliderInt("Rastro", &iRastro, 0, 99);
        if (camera == 2) {
            ImGui::SliderFloat("brilho", &brilhoBrisa, 0, 1);
            ImGui::SliderFloat("contraste", &contrasteBrisa, 0, 1);
        }

    }
}

void FonteKinect::setBlur( int novoBlur ) {
    iBlur = novoBlur;
}
