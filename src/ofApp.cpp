#include "ofApp.h"

vector<Brisa *> brisasAtivas;

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    WindowManager::getInstance().setMainWindowSize(ofGetWidth(), ofGetHeight());

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

//--------------------------------------------------------------
void ofApp::update()
{

    // Atualiza as brisas configuradas
    brisasAtivas = gui->brisasAtivas;
    switch (gui->iBlend)
    {
    case 1:
        gui->blendMode = OF_BLENDMODE_ALPHA;
        break;
    case 2:
        gui->blendMode = OF_BLENDMODE_ADD;
        break;
    case 3:
        gui->blendMode = OF_BLENDMODE_SCREEN;
        break;
    case 4:
        gui->blendMode = OF_BLENDMODE_MULTIPLY;
        break;
    case 5:
        gui->blendMode = OF_BLENDMODE_SUBTRACT;
        break;
    default:
        gui->blendMode = OF_BLENDMODE_ALPHA;
    }

    ofEnableBlendMode(gui->blendMode);

    if (FonteGlobalKinect::getInstance().getIsConnected())
    {
        FonteGlobalKinect::getInstance().update();
    }

    if (gui->isRecording)
    {
        saveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0, 0, 0);
    ofEnableAntiAliasing();
    // Me mostra essas brisa
    for (int i = brisasAtivas.size() - 1; i >= 0; i--)
    {
        brisasAtivas[i]->draw();
    }
    ofEnableAntiAliasing();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    // Libera a memória das brisas
    // Brisa vivida é brisa passada
    for (vector<Brisa *>::iterator i = brisasAtivas.begin(); i != brisasAtivas.end(); ++i)
    {
        brisasAtivas.erase(i);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    WindowManager::getInstance().setMainWindowSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::saveFrame()
{
    ofPixels pixels;
    ofImage screenImage;
    // get current hour and minute
    time_t t = time(0);
    struct tm *now = localtime(&t);
    int hour = now->tm_hour;
    int min = now->tm_min;

    screenImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    screenImage.save("recording/" + ofToString(hour) + "_" + ofToString(min) + "_" + "frame_" + ofToString(frameCount) + ".png");
    gui->setFrameCountRecorded(frameCount);
    frameCount++;
}
