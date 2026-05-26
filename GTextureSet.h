#ifndef G_TEXTURE_SET_H
#define G_TEXTURE_SET_H

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "CommonStructs.h"

/*
    This class holds and manages loaded SDL_Textures for an SDL window
*/

class GTextureSet
{
public:

    GTextureSet();
    bool init(SDL_Renderer* gRenderer);

    ~GTextureSet();
    
    // Clear all textures
    void clear();

    // Get an ID for a loaded texture (load the texture if it has not been loaded yet)
    uint32_t findOrLoadTexture(std::string textureFilePath);

    // Get a pointer to a loaded texture by its ID
    const GTexture* getTexture(uint32_t textureID);

    // Free a loaded texture
    void freeTexture(const std::string& texturePath);

    uint32_t idSystemCounter = 1; // ID 0 is reserved for the fallback texture

private:
    // Flag to check if the class has been initialized
    bool initialized;

    //Load image file as a texture
    uint32_t loadImageTexture(std::string textureFilePath );
    
    // Map of texture file names to their integer IDs
    std::unordered_map<std::string, uint32_t> stringToIDMap;

    // Map of texture IDs to their texture objects
    std::unordered_map<uint32_t, std::unique_ptr<GTexture>> IDToTextureMap;

    // The fallback texture for when an object's weak_ptr is expired
    void createFallbackTexture();

    //Hold pointer to the window's renderer (GTextureSet should have a shorter lifetime then the renderer)
    SDL_Renderer* gRenderer = NULL;

};

#endif