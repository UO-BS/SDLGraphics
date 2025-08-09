#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
#include "GVector.h"
#include "GQuaternion.h"
#include "GMatrix.h"
#include "GRectangleTexture.h"
#include "GTextureSet.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

class MainWindow{

public:
    MainWindow(std::string windowName);
    bool init(std::string windowName);
        
    //Close window (includes cleanup)
    void close();

    //Closes window and cleans up
    ~MainWindow();

private:

    //TODO: Rename to something like "ProjectiveTransfromApproxByAffine"
    std::pair<std::vector<SDL_Vertex>,std::vector<int>> warpTextureBySubdivision(GRectangleTexture text,
																						GMatrix<4,4> position,
																						GMatrix<4,4> cameraDetails);

    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    GTextureSet textureSet; //TextureSet is initialized in init()

};

#endif