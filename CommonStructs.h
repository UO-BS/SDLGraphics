#ifndef G_COMMON_STRUCTS_H
#define G_COMMON_STRUCTS_H

#include <SDL.h>
#include "GVector.h"

struct GTextureDimensions {
    int width;
    int height;
};

struct GTexture {
    SDL_Texture* texture = nullptr;
    GTextureDimensions dimensions{0,0};

    ~GTexture() {
        SDL_DestroyTexture(texture);
    }
};

struct GTextureVertex {
    /*
    Each vertex has a position in world space (x y z w) and a 2D position on the texture image (u v)
    This allows you to squash/stretch/rotate by changing the u v coordinates

    textureVertices are in 0 to 1 range, where (0,0) is the top left
    */
    GVector<4> worldVertex;
    GVector<2> textureVertex;
};

struct GTextureTriangle {
    GTextureVertex vertices[3];
};

#endif