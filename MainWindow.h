#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
#include "GVector.h"
#include "GMatrix.h"
#include "GTextureSet.h"
#include "GTexturePolygon.h"
#include "WorldObject.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

class MainWindow{

public:
    MainWindow(std::string windowName);
    bool init(std::string windowName);
    bool initialized = false;

    // Temporary method to start the main loop
    void run();
        
    //Close window (includes cleanup)
    void close();

    //Closes window and cleans up
    ~MainWindow();

private:

    std::vector<GTexturePolygon> entityToClipSpace(std::vector<WorldObject> worldObjects, WorldObject cameraDetails);

    std::unordered_map<uint32_t, std::vector<SDL_Vertex>> clipSpaceToDrawBatch(std::vector<GTexturePolygon> clipSpacePolygons);

    void draw(std::unordered_map<uint32_t, std::vector<SDL_Vertex>> drawBatches);

    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    GTextureSet textureSet = GTextureSet();

};

#endif