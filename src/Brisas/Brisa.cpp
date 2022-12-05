#include "Brisa.h"

void Brisa::setup() {
    //btnConfigBrisa = gui->loadTexture(iconPath);fboBrisa.clear();
    pixelsBrisa.allocate(WIDTH,HEIGHT, OF_IMAGE_COLOR);
    fboBrisa.allocate(WIDTH, HEIGHT);
    fboBrisa.begin();
    ofClear(255,255,255, 0);
    fboBrisa.end();

    // variaveis do shader
    ligaShader = false;
    p1Shader = 0;
    p2Shader = 0;
    p3Shader = 0;

    clearFrames = true;

    iBrisaShader = 0;

    opacidade = 0;
    // Inicializa variaveis de distorções
    brilhoBrisa = contrasteBrisa = 0.5;
    deslocX = deslocY = 0;
}

void Brisa::update(float dt) {
}

void Brisa::mostraBrisas() {

}

void Brisa::draw() {
}

void Brisa::drawControles(int iBrisa) {
}

void Brisa::desenhaMiniatura(int i, bool ativa) {
    int widthMiniatura = 160;
    int heightMiniatura = 120;
    int border = 2;
    imgBtn.setFromPixels(pixelsBrisa);
    if(ativa) {
        ofSetColor(205, 100, 100);
    } else {
        ofSetColor(30);
    }
    ofRect(0, i * (heightMiniatura + border * 2), widthMiniatura + border * 2, heightMiniatura + border * 2);
    ofSetColor(0);
    ofRect(2, i * (heightMiniatura + border * 2) + border, widthMiniatura, heightMiniatura);
    ofSetColor(255);
    imgBtn.draw(2, i * (heightMiniatura + border * 2) + border, widthMiniatura, heightMiniatura);
}

void Brisa::desenhaJanela(int i) {

    std::ostringstream oss;
    std::string text = "Brisa ";
    text += std::to_string(i);
    
    ImGui::SetNextWindowSize(ofVec2f(500,548), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ofVec2f(210, 430), ImGuiCond_FirstUseEver);
    string titulo = "Brisa #" + to_string(i+1);
    ImGuiWindowFlags window_flags = 0;
  //  window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
  //  window_flags |= ImGuiWindowFlags_NoMove;
    bool janelaAberta = true;
    ImGui::Begin(titulo.c_str(), &janelaAberta, window_flags );

    ImGui::SliderInt("Opacidade", &opacidade, 0, 255);
    drawControles(i);

    if (ImGui::Button("Trazer pra frente")) { trazerFrente(i); } ImGui::SameLine();
    if (ImGui::Button("Esconder pra trás")) { esconderTras(i); } 
    desenharControlesShader();
    if (ImGui::Button("Excluir Brisa")) { excluiBrisa(i); } 

    ImGui::End();
}

void Brisa::esconderTras( int iBrisa ) {
    if( iBrisa >= brisasAtivas->size()-1 ) {
        std::iter_swap(brisasAtivas->begin()+iBrisa, brisasAtivas->begin());
    } else {
        std::iter_swap(brisasAtivas->begin()+iBrisa, brisasAtivas->begin()+iBrisa+1);
    }
}

void Brisa::trazerFrente( int iBrisa ) {
    if( iBrisa <= 0 ) {
        std::iter_swap(brisasAtivas->begin(), brisasAtivas->end()-1);
    } else {
        std::iter_swap(brisasAtivas->begin()+iBrisa, brisasAtivas->begin()+iBrisa-1);
    }

}

void Brisa::excluiBrisa(int iBrisa) {
    brisasAtivas->erase(brisasAtivas->begin()+iBrisa);
}

void Brisa::loadShader(string shader) {
    fragShaderPath = shader;
    shaderBrisa.load("../data/shaders/vertexdummy.c", fragShaderPath);
}

void Brisa::listaBrisas() {

    int nBrisas = brisasAtivas->size();
    if (ImGui::Selectable("Nenhuma")) {
        iBrisaShader = -1;
    }
    for (int i=0; i<nBrisas; i++) {	
        string numBrisa = "Brisa #" + to_string(i+1);
        if (ImGui::Selectable(numBrisa.c_str())) {
            iBrisaShader = i;
        }

    }
}

void Brisa::desenharControlesShader() {
    if (ImGui::CollapsingHeader("Shader")) {
        if (shaderBrisa.isLoaded()) {
            ImGui::Text(fragShaderPath.substr(23).c_str());
            ImGui::Checkbox("Ligar Shader", &ligaShader);
            ImGui::SliderFloat("p1", &p1Shader, 0, 100);
            ImGui::SliderFloat("p2", &p2Shader, 0, 100);
            ImGui::SliderFloat("p3", &p3Shader, 0, 100);
        }
        if (ImGui::Button("Carregar Shader")) {
            ImGui::OpenPopup("loadShader");
        }
        if (ImGui::BeginPopup("loadShader")) {
            listaShaders();
            ImGui::EndPopup();
        }
        string numBrisa = "Brisa Shader: #" + to_string(iBrisaShader + 1);
        if (ImGui::Button(numBrisa.c_str())) {
            ImGui::OpenPopup("selectBrisa");
        }
        if (ImGui::BeginPopup("selectBrisa")) {
            listaBrisas();
            ImGui::EndPopup();
        }
    }
}

void Brisa::listaShaders() {
    if (ImGui::CollapsingHeader("Alpha"))
    {
        ofDirectory dirShaders;
        //2. Carrega numero de pastas de sequencias
        int nShaders = dirShaders.listDir("../data/shaders/alpha");

        //4. Abre pastas
        for (int i = 0; i < nShaders; i++) {
            string shader = dirShaders.getPath(i);
            if (ImGui::Selectable(shader.substr(22).c_str())) {
                loadShader(shader);
            }
        }
    }
    if (ImGui::CollapsingHeader("Mix Brisa"))
    {
        ofDirectory dirShaders;
        //2. Carrega numero de pastas de sequencias
        int nShaders = dirShaders.listDir("../data/shaders/mixbrisa");

        //4. Abre pastas
        for (int i = 0; i < nShaders; i++) {
            string shader = dirShaders.getPath(i);
            if (ImGui::Selectable(shader.substr(25).c_str())) {
                loadShader(shader);
            }
        }
    }
    if (ImGui::CollapsingHeader("Outros"))
    {
        ofDirectory dirShaders;
        //2. Carrega numero de pastas de sequencias
        int nShaders = dirShaders.listDir("../data/shaders/outros");

        //4. Abre pastas
        for (int i = 0; i < nShaders; i++) {
            string shader = dirShaders.getPath(i);
            if (ImGui::Selectable(shader.substr(23).c_str())) {
                loadShader(shader);
            }
        }
    }
}

void Brisa::aplicarShader() {

/*
    while( receiverOSC->hasWaitingMessages() )
    {
        ofxOscMessage m;
        receiverOSC->getNextMessage( &m );
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        float paramOSC;
        for ( int i=0; i<m.getNumArgs(); i++ )
        {
            // get the argument type
            msg_string += " ["+ofToString(i)+"]";
            msg_string += m.getArgTypeName( i );
            msg_string += ":";
            // display the argument - make sure we get the right type
            if( m.getArgType( i ) == OFXOSC_TYPE_INT32 ) {
                msg_string += ofToString( m.getArgAsInt32( i ) );
                paramOSC = ofToFloat(m.getArgAsString(i));
            } else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT ) {
                msg_string += ofToString( m.getArgAsFloat( i ) );
                paramOSC = ofToFloat(m.getArgAsString( i ));
            } else if( m.getArgType( i ) == OFXOSC_TYPE_STRING ) {
                msg_string += m.getArgAsString( i );
                paramOSC = ofToFloat(m.getArgAsString( i ));
            }
            //if( m.getAddress() == "/Note1" && paramOSC == 36) {
            if( m.getAddress() == "pad" && i == 0) {
                if( paramOSC < minp1 ) minp1 = paramOSC;
                if( paramOSC > maxp1 ) maxp1 = paramOSC;
                p1Shader = ofLerp(p1Shader, ofNormalize( paramOSC, minp1, maxp1)*40, 0.1);
                //p1Shader = ofLerp(p1Shader, paramOSC+10, 0.1);
            //} else if ( m.getAddress() == "/Note1" && paramOSC == 46 ) {
            } else if( m.getAddress() == "pad" && i == 1) {
                if( paramOSC < minp2 ) minp2 = paramOSC;
                if( paramOSC > maxp2 ) maxp2 = paramOSC;
                p2Shader = ofLerp(p2Shader, ofNormalize( paramOSC, minp2, maxp2)*40, 0.1);
                //p2Shader = ofLerp(p2Shader, paramOSC+10, 0.1);
            //} else if( m.getAddress() == "/Note1" && paramOSC == 38 ) {
            } else if( m.getAddress() == "vslider" ) {
                if( paramOSC < minp3 ) minp3 = paramOSC;
                if( paramOSC > maxp3 ) maxp3 = paramOSC;
                p3Shader = ofLerp(p3Shader, ofNormalize( paramOSC, minp3, maxp3)*40, 0.5);
                //p3Shader = ofLerp(p3Shader, paramOSC+10, 0.1);
            }
        }

        cout << "\nOSC recebido - Address:" << msg_string;
    }

*/
    
    p1Shader = ofLerp(p1Shader, 0, 0.001);
    p2Shader = ofLerp(p2Shader, 0, 0.001);
    p3Shader = ofLerp(p3Shader, 0, 0.001);

    if (ligaShader) {
        shaderBrisa.begin();
        if (iBrisaShader > -1 && brisasAtivas->at(iBrisaShader)->fboBrisa.isAllocated()) {
            shaderBrisa.setUniformTexture("texture1", brisasAtivas->at(iBrisaShader)->fboBrisa.getTextureReference(), 1); //"1" means that it is texture 1
            shaderBrisa.setUniform1f("p1", p1Shader);
            shaderBrisa.setUniform1f("p2", p2Shader);
            shaderBrisa.setUniform1f("p3", p3Shader);
        }

        ofSetColor(255, 255, 255, opacidade);
        fboBrisa.draw(0, 0);

        shaderBrisa.end();
    }
    else {
        ofSetColor(255, 255, 255, opacidade);
        fboBrisa.draw(0, 0);
    }
}

void Brisa::desenharControlesDistorcao() {
    if (ImGui::CollapsingHeader("Distorções")) {
        ImGui::SliderFloat("brilho", &brilhoBrisa, 0, 1); ImGui::SameLine(); ImGui::Text("n funciona");
        ImGui::SliderFloat("contraste", &contrasteBrisa, 0, 1);ImGui::SameLine(); ImGui::Text("n funciona");
        ImGui::SliderInt("desloca X", &deslocX, -200, 200);
        ImGui::SliderInt("desloca Y", &deslocY, -200, 200);
        ImGui::SliderFloat("Proporcao", &proporcao, 0.2, 2);
        ImGui::SliderFloat("Rotação", &rotacao, -180, 180); ImGui::SameLine();
        ImGui::Checkbox("Automático", &rotacionaSozinho);
        ImGui::Checkbox("Torcer automático", &torceSozinho);
    }
}
