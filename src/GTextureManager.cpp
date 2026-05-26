#include "GEngine/GTextureManager.h"
#include <SDL_ttf.h>

GTextureManager::GTextureManager(): initialized(false) {}

bool GTextureManager::init() {
    // TODO: Handle an init after its already initialized

    createFallbackTexture();

    initialized = true;
    return true;
}

void GTextureManager::createFallbackTexture() {
    // Create the fallback Texture
    std::unique_ptr<GTexture> fallbackTexture = std::make_unique<GTexture>();
    fallbackTexture->width = 2;
    fallbackTexture->height = 2;

    uint8_t fallbackPixels[] = { // Purple and black checkerboard pattern
        255, 0, 255,   
        0, 0, 0,
        0, 0, 0,       
        255, 0, 255
    };

    glGenTextures(1, &fallbackTexture->textureID);
    glBindTexture(GL_TEXTURE_2D, fallbackTexture->textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fallbackTexture->width, fallbackTexture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, fallbackPixels);

    glBindTexture(GL_TEXTURE_2D, 0); 

    textureMap["__fallback__"] = std::move(fallbackTexture);
}

GTextureManager::~GTextureManager() {
    if (!initialized) {return;}

    // Clear all textures
    clear();

    // Destroy fallback Texture
    // Done automatically since its a unique_ptr
}

void GTextureManager::clear() {
    // Clearing the objects from the map will automatically free the texture. 
    //      The map holds a unique pointer to the GTexture struct, which has a destructor that frees the SDL_Texture.
    textureMap.clear();

    initialized = false;
}

const GTexture*  GTextureManager::findOrLoadTexture(std::string textureFilePath) {
    if (!initialized) {
        std::cerr << "GTextureManager not initialized. findOrLoadTexture failed. \n";
        return nullptr;
    }

    auto it = textureMap.find(textureFilePath);
    
    if (it == textureMap.end()) //Texture is not in the map; need to load it
    { 
        return loadImageTexture(textureFilePath);
    } 
    else //Texture found
    { 
        return it->second.get();
    }
}

const GTexture* GTextureManager::loadImageTexture(const std::string& textureFilePath ) {

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( textureFilePath.c_str() );
	if( loadedSurface == NULL )
	{
		std::cerr <<  "Unable to load image" << textureFilePath.c_str() << "\n" ;
        return textureMap["__fallback__"].get();
	}
	
    std::unique_ptr<GTexture> newTexture = std::make_unique<GTexture>();
    newTexture->width = loadedSurface->w;
    newTexture->height = loadedSurface->h;

    glGenTextures(1, &newTexture->textureID);
    glBindTexture(GL_TEXTURE_2D, newTexture->textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // TODO: Consider changing this to GL_NEAREST for Pixel Art
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (loadedSurface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, newTexture->width, newTexture->height, 0, format, GL_UNSIGNED_BYTE, loadedSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean CPU Memory
    SDL_FreeSurface( loadedSurface );
    glBindTexture(GL_TEXTURE_2D, 0); 

    textureMap[textureFilePath] = std::move(newTexture);

    return textureMap[textureFilePath].get();
}

void GTextureManager::freeTexture(const std::string& texturePath) {
    if (!initialized) {
        std::cerr << "GTextureManager not initialized! freeTexture failed. \n" ;
        return;
    }

    // Erasing the object from the map will automatically free the texture. 
    //      The map holds a unique pointer to the GTexture struct, which has a destructor that frees the SDL_Texture.
    textureMap.erase(texturePath);
}