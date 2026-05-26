#ifndef G_COMMON_STRUCTS_H
#define G_COMMON_STRUCTS_H

#include <glad/glad.h>
#include "GVector.h"

struct GTexture {
    GLuint textureID = 0;
    int width = 0;
    int height = 0;

    ~GTexture() {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
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

#endif