#pragma once

class WindowManager {
public:
    static WindowManager& getInstance() {
        static WindowManager instance; // garantido como sendo destruído
        return instance;
    }

    void setMainWindowSize(int width, int height) {
        mainWindowWidth = width;
        mainWindowHeight = height;
    }

    void setGuiWindowSize(int width, int height) {
        guiWindowWidth = width;
        guiWindowHeight = height;
    }

    int getMainWindowWidth() { return mainWindowWidth; }
    int getMainWindowHeight() { return mainWindowHeight; }
    int getGuiWindowWidth() { return guiWindowWidth; }
    int getGuiWindowHeight() { return guiWindowHeight; }

private:
    WindowManager() {} // Construtor privado
    int mainWindowWidth = 1024;
    int mainWindowHeight = 768;
    int guiWindowWidth = 1500;
    int guiWindowHeight = 800;
};