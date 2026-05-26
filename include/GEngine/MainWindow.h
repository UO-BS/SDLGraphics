#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "GAssetManager.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

class MainWindow{

public:

    MainWindow(std::string windowName);

    //Closes window and cleans up
    ~MainWindow();

    bool init(std::string windowName);
    bool initialized = false;



    // Temporary method to start the main loop
    void run();
        
    //Close window (includes cleanup)
    void close();

    // No Copying
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

private:

    // SDL and OpenGL handles
    SDL_Window* gWindow = nullptr;
    SDL_GLContext gContext = nullptr;

    // The asset manager
    GAssetManager assetManager;

    GLuint shaderProgramID = 0;      // Tracks the compiled GPU program
    GLint shaderMatrixLocation = -1; // Slot ID for individual object positions
    GLint shaderViewProjLocation = -1; // Slot ID for global camera lens metrics
    
};

#endif