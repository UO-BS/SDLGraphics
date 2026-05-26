#ifndef G_TEXTURE_MANAGER_H
#define G_TEXTURE_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "GCommonStructs.h"

/*
    Manage loaded textures
    Holds unique_ptrs to all GTexture objects
*/

class GTextureManager
{
public:

    GTextureManager();
    ~GTextureManager();

    bool init();

    // Clear all textures (Ensure that all objects have been cleared first)
    void clear(); 

    // Get a loaded texture (load the texture if it has not been loaded yet)
    // Will return a nullptr if the GTextureManager is not initialized
    // Will return the fallback texture if the desired texture is not found
    const GTexture* findOrLoadTexture(std::string textureFilePath);

    // Free a loaded texture (Ensure that all objects that use this texture have been cleared first)
    void freeTexture(const std::string& texturePath);

private:
    // Flag to check if the class has been initialized
    bool initialized;

    //Load image file as a texture 
    const GTexture* loadImageTexture(const std::string& textureFilePath );
    
    // Map of texture file names to their textures
    std::unordered_map<std::string, std::unique_ptr<GTexture>> textureMap;

    // The fallback texture for when an object's weak_ptr is expired
    void createFallbackTexture();

};

#endif