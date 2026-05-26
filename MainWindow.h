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

    void clipTriangles(std::vector<GTexturePolygon>& polygons);

    std::unordered_map<uint32_t, std::vector<SDL_Vertex>> clipSpaceToDrawBatch(std::vector<GTexturePolygon> clipSpacePolygons);

    void draw(std::unordered_map<uint32_t, std::vector<SDL_Vertex>> drawBatches);

    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    GTextureSet textureSet = GTextureSet();

    uint8_t computeOutCode(GVector<4> vertex);
    enum PlaneIndex: uint8_t {
        NEAR_PLANE = 0 << 0, // 0b00001
        LEFT_PLANE = 1 << 1, // 0b00010
        RIGHT_PLANE = 1 << 2, // 0b00100
        TOP_PLANE = 1 << 3, // 0b01000
        BOTTOM_PLANE = 1 << 4, // 0b10000
        FAR_PLANE = 1 << 5 // 0b100000
    };
    static constexpr int planeComponentMap[6] = {2,0,0,1,1,2}; 
    static constexpr float signMap[6] = {1, 1, -1, -1, 1, -1};

};

#endif