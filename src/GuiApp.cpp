#include "GuiApp.h"
#include <vector>
using namespace std;

float t0, t1;

void GuiApp::setup()
{
    WindowManager::getInstance().setGuiWindowSize(ofGetWidth(), ofGetHeight());

    gui.setup();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 5;
    // change the main color of buttons, headers, etc
    // style.Colors[ImGuiCol_Button] = ImVec4(0.4, 0.2, 0.8, 1.0);

    // change the collapsed header style
    style.Colors[ImGuiCol_Header] = ImVec4(0.3, 0.0, 0.5, 1.0);

    btnCriaColunas = gui.loadTexture("../data/img/icon/colunas.png");
    btnCriaVideo = gui.loadTexture("../data/img/icon/video.png");
    btnCriaPoligono = gui.loadTexture("../data/img/icon/poligon.png");
    btnCriaKinect = gui.loadTexture("../data/img/icon/kinect.png");
    btnCriaImg = gui.loadTexture("../data/img/icon/img.png");
    btnCriaGif = gui.loadTexture("../data/img/icon/gif.png");
    btnCriaMatriz = gui.loadTexture("../data/img/icon/matriz.png");
    btnCriaSombras = gui.loadTexture("../data/img/icon/sombras.png");
    btnCriaMandala = gui.loadTexture("../data/img/icon/mandala.png");
    btnCriaBorda = gui.loadTexture("../data/img/icon/borda.png");
    btnCriaKinectCam = gui.loadTexture("../data/img/icon/kinectcam.png");
    btnCriaCamera = gui.loadTexture("../data/img/icon/camera.png");
    btnCriaBody = gui.loadTexture("../data/img/icon/camera.png");
    btnCriaFinder = gui.loadTexture("../data/img/icon/camera.png");

    ImGui::GetIO().MouseDrawCursor = false;
    desenhaMiniaturas = true;

    coresPaleta.push_back(ofColor::fromHex(0x00126D));
    coresPaleta.push_back(ofColor::fromHex(0xC0126D));
    coresPaleta.push_back(ofColor::fromHex(0x000000));

    iBlend = 2;
    anguloKinect = 0;
    depthClippingFar = 2000;
    depthClippingNear = 500;
}

void GuiApp::update()
{
    // Quanto tempo passou desde o ultimo update?
    // Resp = t1 - t0
    t0 = t1;
    t1 = ofGetElapsedTimef();
    float dt = t1 - t0;

    // Atualiza as brisa tudo
    for (int i = 0; i < brisasAtivas.size(); i++)
    {
        brisasAtivas[i]->update(dt);
    }
}

void GuiApp::draw()
{
    ofBackground(100, 100, 100);

    gui.begin();

    // if the window is resized, update the size of the gui
    ImVec2 currentWindowSize = ImGui::GetWindowSize();

    // Janela para adicionar brisas
    ImGui::SetNextWindowSize(ofVec2f(550, 380), ImGuiCond_Appearing);
    adicionaBrisa();

    ImGui::SetNextWindowSize(ofVec2f(250, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ofVec2f(1060, 2), ImGuiCond_FirstUseEver);
    UIControlesGerais();

    // Me mostra essas brisa
    for (int i = 0; i < brisasAtivas.size(); i++)
    {
        if (desenhaMiniaturas)
        {
            bool focada = (i == iBrisaFocada);
            brisasAtivas[i]->desenhaMiniatura(i, focada);
        }
        if (i == iBrisaFocada)
        {
            brisasAtivas[i]->desenhaJanela(i);
        }
    }

    for (int i = 0; i < timeline0.size(); i++)
    {
        bool focada = false;
        timeline0[i]->desenhaMiniatura(i, focada);
    }

    ImGui::End();

    gui.end();

    // Desenha as imagens do kinect
    ofTexture depthTexture = FonteGlobalKinect::getInstance().getDepthTexture();
    if (depthTexture.isAllocated() && FonteGlobalKinect::getInstance().getIsConnected())
    {
        int widthMiniatura = 160;
        int heightMiniatura = 120;
        // bottom right
        int x = ofGetWidth() - widthMiniatura - 2;
        int y = ofGetHeight() - heightMiniatura - 2;
        // draw the depth texture
        depthTexture.draw(x, y, widthMiniatura, heightMiniatura);

        // draw the rgb texture
        ofTexture rgbTexture = FonteGlobalKinect::getInstance().getRgbTexture();
        rgbTexture.draw(x, y - 2 - heightMiniatura, widthMiniatura, heightMiniatura);
    }
}

void GuiApp::UIControlesGerais()
{
    ImGui::Begin("Controles Gerais");

    ImGui::Text("%.1f FPS (%.3f ms/frame) ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Checkbox("Desenha miniaturas", &desenhaMiniaturas);

    if (ImGui::CollapsingHeader("Kinect"))
    {
        if (FonteGlobalKinect::getInstance().getIsConnected())
        {
            // Atualiza o angulo do kinect
            FonteGlobalKinect::getInstance().setAngle(anguloKinect);
            // Atualiza o corte de profundidade
            FonteGlobalKinect::getInstance().setDepthClipping(depthClippingNear, depthClippingFar);
        }
        // Botões de liga e desliga do kinect
        if (FonteGlobalKinect::getInstance().getIsConnected())
        {
            ImGui::SliderInt("angulo", &anguloKinect, -30, 30);
            ImGui::SliderInt("depthClippingNear", &depthClippingNear, 500, 4000);
            ImGui::SliderInt("depthClippingFar", &depthClippingFar, 500, 4000);
            if (ImGui::Button("Desliga Kinect Global"))
            {
                FonteGlobalKinect::getInstance().desligaKinect();
            }
        }
        else
        {
            if (ImGui::Button("Liga Kinect Global"))
            {
                FonteGlobalKinect::getInstance().ligaKinect();
            }
        }
    }

    if (ImGui::CollapsingHeader("Paleta de cores"))
    {
        for (int i = 0; i < coresPaleta.size(); i++)
        {
            string nomeCor = "Cor " + to_string(i + 1);
            ImGui::ColorEdit4(nomeCor.c_str(), (float *)&coresPaleta[i]);
            ImGui::SameLine();
            nomeCor = "[X] Excluir " + to_string(i + 1);
            if (ImGui::Button(nomeCor.c_str()))
            {
                coresPaleta.erase(coresPaleta.begin() + i);
            }
        }

        if (ImGui::Button(" + "))
        {
            coresPaleta.push_back(ofColor::fromHex(0xC0126D));
        }
    }

    ImGui::Text("Blend Mode");
    ImGui::RadioButton("alpha", &iBlend, 1);
    ImGui::SameLine();
    ImGui::RadioButton("add", &iBlend, 2);
    ImGui::SameLine();
    ImGui::RadioButton("screen", &iBlend, 3);

    // Pula uma linha
    ImGui::NewLine();

    // REC main screen
    if (isRecording)
    {
        ImGui::Text("Recording... %d", framesRecorded);
        if (ImGui::Button("[STOP]"))
        {
            stopRecording();
        }
    }
    else
    {
        if (ImGui::Button("[REC]"))
        {
            startRecording();
        }
    }
}

void GuiApp::setFrameCountRecorded(int frameCount)
{
    this->framesRecorded = frameCount;
}

void GuiApp::startRecording()
{
    isRecording = true;
    framesRecorded = 0;
}

void GuiApp::stopRecording()
{
    isRecording = false;
}

void GuiApp::adicionaBrisa()
{

    ImGui::SetNextWindowSize(ofVec2f(570, 340), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ofVec2f(170, 2), ImGuiCond_FirstUseEver);
    ImGui::Begin("Adicione uma camada de brisa:");
    bool criaColunas = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaColunas, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaKinect = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaKinect, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaKinectCam = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaKinectCam, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaMatriz = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaMatriz, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaSombras = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaSombras, ImVec2(120, 90));

    bool criaGif = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaGif, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaImg = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaImg, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaPoligono = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaPoligono, ImVec2(120, 90));
    ImGui::SameLine();
    bool criaVideo = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaVideo, ImVec2(120, 90));

    bool criaCamera = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaCamera, ImVec2(120, 90));
    // ImGui::SameLine();
    // bool criaFinder = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaFinder, ImVec2(120, 90));
    // ImGui::SameLine();
    // bool criaBody = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaBody, ImVec2(120, 90));
    //    bool criaMandala = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaMandala, ImVec2(120, 90)); ImGui::SameLine();
    //    bool criaBorda = ImGui::ImageButton((ImTextureID)(uintptr_t)btnCriaBorda, ImVec2(120, 90));

    if (criaColunas)
    {
        cout << "btn pressionado: criaColunas";
        brisasAtivas.push_back(new Colunas());
    }
    if (criaVideo)
    {
        cout << "btn pressionado: criaVideo";
        brisasAtivas.push_back(new VideoBrisa(&brisasAtivas));
    }
    if (criaPoligono)
    {
        cout << "btn pressionado: criaPoligono";
        brisasAtivas.push_back(new PoligonoBrisa(&brisasAtivas, &coresPaleta));
    }
    if (criaKinect)
    {
        cout << "btn pressionado: criaKinect";
        brisasAtivas.push_back(new KinectContornosBrisa());
    }
    if (criaImg)
    {
        cout << "btn pressionado: criaImg";
        brisasAtivas.push_back(new ImagemBrisa(&brisasAtivas));
    }
    if (criaGif)
    {
        cout << "btn pressionado: criaGif";
        brisasAtivas.push_back(new GifBrisa(&brisasAtivas));
    }
    if (criaMatriz)
    {
        cout << "btn pressionado: criaMatriz";
        brisasAtivas.push_back(new MatrizBrisa(&kinectGlobal, &brisasAtivas, coresPaleta));
    }
    if (criaSombras)
    {
        cout << "btn pressionado: criaSombras";
        brisasAtivas.push_back(new SombraBrisa(&kinectGlobal, &brisasAtivas, &coresPaleta));
    }
    if (criaKinectCam)
    {
        cout << "btn pressionado: criaKinectCam";
        brisasAtivas.push_back(new KinectCamBrisa(&kinectGlobal, &brisasAtivas));
    }
    if (criaCamera)
    {
        cout << "btn pressionado: criaCamera";
        brisasAtivas.push_back(new CameraBrisa(&brisasAtivas));
    }
    // if (criaFinder) {
    //     cout << "btn pressionado: criaBody";
    //     // brisasAtivas.push_back(new FinderBrisa(&brisasAtivas, &coresPaleta));
    // }
    // if (criaBody) {
    //     cout << "btn pressionado: criaBody";
    //     // brisasAtivas.push_back(new BodyBrisa(&brisasAtivas, &coresPaleta));
    // }

    ImGui::End();
}

void GuiApp::mousePressed(int x, int y, int iButton)
{
    int widthMiniatura = 160;
    int heightMiniatura = 120;
    int qtd = 4;
    if (y < heightMiniatura * qtd && x < widthMiniatura)
    {
        iBrisaFocada = floor((float)y / heightMiniatura);
        cout << "\nx:" << x << " y:" << y << " btn:" << iButton;
        cout << " - Brisa Focada: " << iBrisaFocada;
    }
}

void GuiApp::windowResized(int w, int h)
{
    WindowManager::getInstance().setGuiWindowSize(w, h);
}