#include "GTextureSet.h"
#include <SDL_ttf.h>

GTextureSet::GTextureSet(): initialized(false) {}

bool GTextureSet::init(SDL_Renderer* gRenderer) {
    //Set the renderer
    this->gRenderer = gRenderer;

    //Initialization flag
	bool success = true;
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    initialized = success;
    return success;
}

GTextureSet::~GTextureSet() {
    if (!initialized) {return;}

    //Delete all textures
    for (auto texture : gTextures) {
        SDL_DestroyTexture(texture);
    }
    gTextures.clear();
    gMap.clear();

    //Quit SDL_ttf
    TTF_Quit();
}

int GTextureSet::findOrLoadTexture(std::string textureFilePath) {
    if (!initialized) {
        printf( "GTextureSet not initialized! findOrLoadTexture failed. \n" );
        return -1;
    }

    auto it = gMap.find(textureFilePath);
    
    if (it == gMap.end()) //Texture is not in the map; need to load it
    { 
        return loadImageTexture(textureFilePath);
    } 
    else //Texture found
    { 
        return it->second;
    }
}

SDL_Texture* GTextureSet::getTexture(int index) {
    if (!initialized) {
        printf( "GTextureSet not initialized! getTexture failed. \n" );
        return nullptr;
    }
    if (index > gTextures.size()-1) {
        printf( "Invalid Index! getTexture failed. \n" );
        return nullptr;
    }

    return gTextures[index];
}

int GTextureSet::loadImageTexture( std::string path ) {
    //The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str() );
        return -1;
	}
	
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
        return -1;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    gTextures.push_back(newTexture);
    size_t pos = gTextures.size() - 1;
    gMap[path] = pos;

	return pos;
}

void GTextureSet::freeTexture(std::string texturePath) {
    if (!initialized) {
        printf( "GTextureSet not initialized! freeTexture failed. \n" );
        return;
    }

    auto it = gMap.find(texturePath);
    
    //Texture is not in the map
    if (it == gMap.end()) {
        return;
    }

    //Destroy texture
    SDL_DestroyTexture(gTextures[it->second]);
    //Remove from loaded texture vector
	gTextures.erase(std::next(gTextures.begin(),it->second));
    //Remove from map
    gMap.erase(it);
}