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

    int getMainWindowWidth() { return mainWindowWidth; }
    int getMainWindowHeight() { return mainWindowHeight; }

private:
    WindowManager() {} // Construtor privado
    int mainWindowWidth = 1024;
    int mainWindowHeight = 768;
};