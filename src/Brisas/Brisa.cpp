#include "Brisa.h"

void Brisa::setup()
{
    // btnConfigBrisa = gui->loadTexture(iconPath);fboBrisa.clear();

    int width = WindowManager::getInstance().getMainWindowWidth();
    int height = WindowManager::getInstance().getMainWindowHeight();

    pixelsBrisa.allocate(width, height, OF_IMAGE_COLOR);
    fboBrisa.allocate(width, height, GL_RGBA);
    fboBrisa.begin();
    ofClear(0,0,0, 0);
    fboBrisa.end();

    // variaveis do shader
    ligaShader = false;

    clearFrames = true;

    iBrisaShader = 0;

    opacidade = 0;
    // Inicializa variaveis de distorções
    brilhoBrisa = contrasteBrisa = 0.5;
    deslocX = deslocY = 0; 
    rotacao = 0;
    proporcao = 1;
    rotacionaSozinho = false;
}

void Brisa::update(float dt)
{
}

void Brisa::mostraBrisas()
{
}

void Brisa::draw()
{
    int width = WindowManager::getInstance().getMainWindowWidth();
    int height = WindowManager::getInstance().getMainWindowHeight();

    if (ligaShader)
    {
        shaderBrisa.begin();
        // Para pegar a textura de outra brisa
        // if (iBrisaShader > -1 && brisasAtivas->at(iBrisaShader)->fboBrisa.isAllocated())
        // {
        //     shaderBrisa.setUniformTexture("texture1", brisasAtivas->at(iBrisaShader)->fboBrisa.getTextureReference(), 1);
        // }
        shaderBrisa.setUniformTexture("texture0", fboBrisa.getTextureReference(), 0);
        shaderBrisa.setUniform2f("resolution", fboBrisa.getWidth(), fboBrisa.getHeight());

        ofSetColor(255, 255, 255, opacidade);

        ofPushMatrix();
        ofTranslate(width/2 + deslocX, height/2 + deslocY, 0);
        fboBrisa.setAnchorPercent(0.5, 0.5);
        if (rotacionaSozinho) {
            rotacao += ofNoise(ofGetElapsedTimef());
            if (rotacao > 360) rotacao = 0;
        }
        ofRotate(rotacao);

        ofScale(proporcao, proporcao, 1);
        fboBrisa.draw(0, 0);
        ofPopMatrix();
        shaderBrisa.end();
    }
    else
    {
        ofSetColor(255, 255, 255, opacidade);
        fboBrisa.draw(0, 0);
    }
}

void Brisa::drawControles(int iBrisa)
{
    desenharControlesPosicao();
}

void Brisa::desenhaMiniatura(int i, bool ativa)
{
    int widthMiniatura = 160;
    int heightMiniatura = 120;
    int border = 2;
    imgBtn.setFromPixels(pixelsBrisa);
    if (ativa)
    {
        ofSetColor(205, 100, 100);
    }
    else
    {
        ofSetColor(30);
    }
    ofRect(0, i * (heightMiniatura + border * 2), widthMiniatura + border * 2, heightMiniatura + border * 2);
    ofSetColor(0);
    ofRect(2, i * (heightMiniatura + border * 2) + border, widthMiniatura, heightMiniatura);
    ofSetColor(255);
    imgBtn.draw(2, i * (heightMiniatura + border * 2) + border, widthMiniatura, heightMiniatura);
}

void Brisa::desenhaJanela(int i)
{

    std::ostringstream oss;
    std::string text = "Brisa ";
    text += std::to_string(i);

    ImGui::SetNextWindowSize(ofVec2f(500, 548), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ofVec2f(210, 430), ImGuiCond_FirstUseEver);
    string titulo = "Brisa #" + to_string(i + 1);
    ImGuiWindowFlags window_flags = 0;
    //  window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    //  window_flags |= ImGuiWindowFlags_NoMove;
    bool janelaAberta = true;
    ImGui::Begin(titulo.c_str(), &janelaAberta, window_flags);

    ImGui::SliderInt("Opacidade", &opacidade, 0, 255);
    drawControles(i);

    if (ImGui::Button("Trazer pra frente"))
    {
        trazerFrente(i);
    }
    ImGui::SameLine();
    if (ImGui::Button("Esconder pra trás"))
    {
        esconderTras(i);
    }
    desenharControlesShader();
    if (ImGui::Button("Excluir Brisa"))
    {
        excluiBrisa(i);
    }

    ImGui::End();
}

void Brisa::esconderTras(int iBrisa)
{
    if (iBrisa >= brisasAtivas->size() - 1)
    {
        std::iter_swap(brisasAtivas->begin() + iBrisa, brisasAtivas->begin());
    }
    else
    {
        std::iter_swap(brisasAtivas->begin() + iBrisa, brisasAtivas->begin() + iBrisa + 1);
    }
}

void Brisa::trazerFrente(int iBrisa)
{
    if (iBrisa <= 0)
    {
        std::iter_swap(brisasAtivas->begin(), brisasAtivas->end() - 1);
    }
    else
    {
        std::iter_swap(brisasAtivas->begin() + iBrisa, brisasAtivas->begin() + iBrisa - 1);
    }
}

void Brisa::excluiBrisa(int iBrisa)
{
    brisasAtivas->erase(brisasAtivas->begin() + iBrisa);
}

void Brisa::loadShader(string shader)
{
    fragShaderPath = shader;
    shaderBrisa.load("../data/shaders/vertexdummy.c", fragShaderPath);
}

void Brisa::listaBrisas()
{

    int nBrisas = brisasAtivas->size();
    if (ImGui::Selectable("Nenhuma"))
    {
        iBrisaShader = -1;
    }
    for (int i = 0; i < nBrisas; i++)
    {
        string numBrisa = "Brisa #" + to_string(i + 1);
        if (ImGui::Selectable(numBrisa.c_str()))
        {
            iBrisaShader = i;
        }
    }
}

void Brisa::desenharControlesShader()
{
    if (ImGui::CollapsingHeader("Shader"))
    {
        if (shaderBrisa.isLoaded())
        {
            ImGui::Text(fragShaderPath.substr(23).c_str());
            ImGui::Checkbox("Ligar Shader", &ligaShader);
        }
        if (ImGui::Button("Carregar Shader"))
        {
            ImGui::OpenPopup("loadShader");
        }
        if (ImGui::BeginPopup("loadShader"))
        {
            listaShaders();
            ImGui::EndPopup();
        }
        string numBrisa = "Brisa Shader: #" + to_string(iBrisaShader + 1);
        if (ImGui::Button(numBrisa.c_str()))
        {
            ImGui::OpenPopup("selectBrisa");
        }
        if (ImGui::BeginPopup("selectBrisa"))
        {
            listaBrisas();
            ImGui::EndPopup();
        }
    }
}

void Brisa::listaShaders()
{
    if (ImGui::CollapsingHeader("Alpha"))
    {
        ofDirectory dirShaders;
        // 2. Carrega numero de pastas de sequencias
        int nShaders = dirShaders.listDir("../data/shaders/alpha");

        // 4. Abre pastas
        for (int i = 0; i < nShaders; i++)
        {
            string shader = dirShaders.getPath(i);
            if (ImGui::Selectable(shader.substr(22).c_str()))
            {
                loadShader(shader);
            }
        }
    }
    if (ImGui::CollapsingHeader("Mix Brisa"))
    {
        ofDirectory dirShaders;
        // 2. Carrega numero de pastas de sequencias
        int nShaders = dirShaders.listDir("../data/shaders/mixbrisa");

        // 4. Abre pastas
        for (int i = 0; i < nShaders; i++)
        {
            string shader = dirShaders.getPath(i);
            if (ImGui::Selectable(shader.substr(25).c_str()))
            {
                loadShader(shader);
            }
        }
    }
    if (ImGui::CollapsingHeader("Outros"))
    {
        ofDirectory dirShaders;
        // 2. Carrega numero de pastas de sequencias
        int nShaders = dirShaders.listDir("../data/shaders/outros");

        // 4. Abre pastas
        for (int i = 0; i < nShaders; i++)
        {
            string shader = dirShaders.getPath(i);
            if (ImGui::Selectable(shader.substr(23).c_str()))
            {
                loadShader(shader);
            }
        }
    }
}

void Brisa::desenharControlesPosicao()
{
    if (ImGui::CollapsingHeader("Posição e Tamanho"))
    {
        int width = WindowManager::getInstance().getMainWindowWidth();
        int height = WindowManager::getInstance().getMainWindowHeight();

        ImGui::SliderFloat("brilho", &brilhoBrisa, 0, 1);
        ImGui::SameLine();
        ImGui::Text("n funciona");
        ImGui::SliderFloat("contraste", &contrasteBrisa, 0, 1);
        ImGui::SameLine();
        ImGui::Text("n funciona");
        ImGui::SliderInt("desloca X", &deslocX, -200, 200);
        ImGui::SliderInt("desloca Y", &deslocY, -200, 200);
        ImGui::SliderFloat("Proporcao", &proporcao, 0.2, 2);
        ImGui::SliderFloat("Rotação", &rotacao, -180, 180);
        ImGui::SameLine();
        ImGui::Checkbox("Automático", &rotacionaSozinho);
    }
}
