#ifndef G_TEXTURE_SET_H
#define G_TEXTURE_SET_H

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>

// This class holds and manages loadded textures for a window

class GTextureSet
{
public:

    GTextureSet();
    bool init(SDL_Renderer* gRenderer);

    ~GTextureSet();

    // Get the index of loaded texture (load the texture if it has not been loaded yet)
    int findOrLoadTexture(std::string textureFilePath);

    //Get texture based on index
    //TODO: Create a missing texture and return it for invalid index
    SDL_Texture* getTexture(int index);

    // Remove loaded texture (potentially dangerous)
    //TODO: right now it uses erase(), which will invalidate all indices after the erased one... This is bad
    //Solution: only 1 set; whenever i need to edit the text, just overwrite the texture in vector[] 
        // Never remove any textures; only replace
    void freeTexture(std::string texturePath);

private:

    bool initialized;

    //Load image file as a texture
    int loadImageTexture( std::string path );
    
    //Currently loaded textures
    std::vector<SDL_Texture*> gTextures;
    // Map of texture file names to the loaded texture index
    std::unordered_map<std::string,int> gMap;

    //Hold pointer to the window's renderer (GTextureSet should have a shorter lifetime then the renderer)
    SDL_Renderer* gRenderer = NULL;

};

#endif