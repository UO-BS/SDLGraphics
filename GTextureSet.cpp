#include "GTextureSet.h"
#include <SDL_ttf.h>

GTextureSet::GTextureSet(): initialized(false) {}

bool GTextureSet::init(SDL_Renderer* gRenderer) {
    //Set the renderer
    this->gRenderer = gRenderer;

    createFallbackTexture();

    initialized = true;
    return true;
}

void GTextureSet::createFallbackTexture() {
    // Create the fallback Texture
    std::unique_ptr<GTexture> fallbackTexture = std::make_unique<GTexture>();
    fallbackTexture->texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 16, 16);
    fallbackTexture->dimensions.width = 16;
    fallbackTexture->dimensions.height = 16;
    // Draw the fallback Texture (purple and black checkerboard)
    SDL_SetRenderTarget(gRenderer, fallbackTexture->texture);
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255); // Set to purple
    SDL_RenderClear(gRenderer); // Color the whole texture purple
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Set to black
    SDL_Rect cornerRect = {0,0,8,8};
    SDL_RenderFillRect(gRenderer, &cornerRect); // Fill corner with black
    SDL_Rect otherCornerRect = {8,8,8,8};
    SDL_RenderFillRect(gRenderer, &otherCornerRect); // Fill corner with black
    // Reset the render target
    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_SetRenderDrawColor(gRenderer, 255, 2550, 255, 255); // Set to white for future draws

    IDToTextureMap[0] = std::move(fallbackTexture);
}

GTextureSet::~GTextureSet() {
    if (!initialized) {return;}

    // Clear all textures
    clear();
}

void GTextureSet::clear() {
    if (!initialized) {return;}

    // Clearing the objects from the map will automatically free the texture. 
    //      The map holds a unique pointer to the GTexture struct, which has a destructor that frees the SDL_Texture.
    stringToIDMap.clear();
    IDToTextureMap.clear();
}

const GTexture* GTextureSet::getTexture(uint32_t textureID) {
    if (!initialized) {
        printf( "GTextureSet not initialized! getTexture failed. \n" );
        return nullptr;
    }

    auto it = IDToTextureMap.find(textureID);
    
    if (it == IDToTextureMap.end()) //Texture is not in the map; use placeholder
    { 
        printf( "Texture ID %d not found! getTexture failed. \n", textureID );
        return IDToTextureMap[0].get();
    } 
    else //Texture found
    { 
        return it->second.get();
    }
}

uint32_t GTextureSet::findOrLoadTexture(std::string textureFilePath) {
    if (!initialized) {
        printf( "GTextureSet not initialized! findOrLoadTexture failed. \n" );
        return -1;
    }

    auto it = stringToIDMap.find(textureFilePath);
    
    if (it == stringToIDMap.end()) //Texture is not in the map; need to load it
    { 
        return loadImageTexture(textureFilePath);
    } 
    else //Texture found
    { 
        return it->second;
    }
}

uint32_t GTextureSet::loadImageTexture( std::string textureFilePath ) {
    //The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( textureFilePath.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", textureFilePath.c_str() );
        return -1;
	}
	
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error: %s\n", textureFilePath.c_str(), SDL_GetError() );
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
        return -1;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    // Create unique pointer to the GTexture struct
    std::unique_ptr<GTexture> newGTexture = std::make_unique<GTexture>();
    newGTexture->texture = newTexture;
    newGTexture->dimensions.width = loadedSurface->w;
    newGTexture->dimensions.height = loadedSurface->h;

    uint32_t newID = idSystemCounter++;
    stringToIDMap[textureFilePath] = newID;
    IDToTextureMap[newID] = std::move(newGTexture);
    return newID;
}

void GTextureSet::freeTexture(const std::string& texturePath) {
    if (!initialized) {
        printf( "GTextureSet not initialized! freeTexture failed. \n" );
        return;
    }

    // Erasing the object from the map will automatically free the texture. 
    //      The map holds a unique pointer to the GTexture struct, which has a destructor that frees the SDL_Texture.
    stringToIDMap.erase(texturePath);
    IDToTextureMap.erase(stringToIDMap[texturePath]);
}