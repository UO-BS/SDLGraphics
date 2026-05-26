#include "GEngine/GMeshBuilder.h"

namespace GMeshBuilder {
    std::vector<GTextureVertex> CreateRectangle(float width, float height) {
        std::vector<GTextureVertex> vertices;

        // Calculate the outer edges relative to the center origin (0,0)
        float halfW = width / 2.0f;
        float halfH = height / 2.0f;

        // Triangle 1 (Top-Left, Bottom-Left, Top-Right)
        // Top-Left corner
        GTextureVertex tl{GVector<4>{ -halfW,  halfH, 0.0f, 1.0f }, GVector<2>{ 0.0f, 0.0f }};
        vertices.push_back(tl);
        // Bottom-Left corner
        GTextureVertex bl{GVector<4>{ -halfW, -halfH, 0.0f, 1.0f }, GVector<2>{ 0.0f, 1.0f }};
        vertices.push_back(bl);
        // Top-Right corner
        GTextureVertex tr{GVector<4>{  halfW,  halfH, 0.0f, 1.0f }, GVector<2>{ 1.0f, 0.0f }};
        vertices.push_back(tr);

        // Triangle 2 (Top-Right, Bottom-Left, Bottom-Right)
        // Top-Right corner  
        vertices.push_back(tr);
        // Bottom-Left corner
        vertices.push_back(bl);
        // Bottom-Right corner
        GTextureVertex br{GVector<4>{  halfW, -halfH, 0.0f, 1.0f }, GVector<2>{ 1.0f, 1.0f }};
        vertices.push_back(br);

        return vertices;
    }


    std::vector<GTextureVertex> CreateCube(float width, float height, float depth) {
        std::vector<GTextureVertex> vertices;

        float w = width  / 2.0f;
        float h = height / 2.0f;
        float d = depth  / 2.0f;

        // Helper macro to reduce boilerplate when building vertices.
        // Maps: X, Y, Z, W  and  U, V
        auto make_v = [](float x, float y, float z, float u, float v) {
            GTextureVertex vert;
            vert.worldVertex   = GVector<4>{ x, y, z, 1.0f };
            vert.textureVertex = GVector<2>{ u, v };
            return vert;
        };

        // FACE 1: FRONT (Facing positive Z)
        vertices.push_back(make_v(-w,  h,  d, 0.0f, 0.0f)); // Top-Left
        vertices.push_back(make_v(-w, -h,  d, 0.0f, 1.0f)); // Bottom-Left
        vertices.push_back(make_v( w,  h,  d, 1.0f, 0.0f)); // Top-Right
        vertices.push_back(make_v( w,  h,  d, 1.0f, 0.0f)); // Top-Right
        vertices.push_back(make_v(-w, -h,  d, 0.0f, 1.0f)); // Bottom-Left
        vertices.push_back(make_v( w, -h,  d, 1.0f, 1.0f)); // Bottom-Right

        // FACE 2: BACK (Facing negative Z)
        vertices.push_back(make_v( w,  h, -d, 0.0f, 0.0f)); // Top-Left (from back perspective)
        vertices.push_back(make_v( w, -h, -d, 0.0f, 1.0f)); // Bottom-Left
        vertices.push_back(make_v(-w,  h, -d, 1.0f, 0.0f)); // Top-Right
        vertices.push_back(make_v(-w,  h, -d, 1.0f, 0.0f)); // Top-Right
        vertices.push_back(make_v( w, -h, -d, 0.0f, 1.0f)); // Bottom-Left
        vertices.push_back(make_v(-w, -h, -d, 1.0f, 1.0f)); // Bottom-Right

        // FACE 3: LEFT (Facing negative X)
        vertices.push_back(make_v(-w,  h, -d, 0.0f, 0.0f));
        vertices.push_back(make_v(-w, -h, -d, 0.0f, 1.0f));
        vertices.push_back(make_v(-w,  h,  d, 1.0f, 0.0f));
        vertices.push_back(make_v(-w,  h,  d, 1.0f, 0.0f));
        vertices.push_back(make_v(-w, -h, -d, 0.0f, 1.0f));
        vertices.push_back(make_v(-w, -h,  d, 1.0f, 1.0f));

        // FACE 4: RIGHT (Facing positive X)
        vertices.push_back(make_v( w,  h,  d, 0.0f, 0.0f));
        vertices.push_back(make_v( w, -h,  d, 0.0f, 1.0f));
        vertices.push_back(make_v( w,  h, -d, 1.0f, 0.0f));
        vertices.push_back(make_v( w,  h, -d, 1.0f, 0.0f));
        vertices.push_back(make_v( w, -h,  d, 0.0f, 1.0f));
        vertices.push_back(make_v( w, -h, -d, 1.0f, 1.0f));

        // FACE 5: TOP (Facing positive Y)
        vertices.push_back(make_v(-w,  h, -d, 0.0f, 0.0f));
        vertices.push_back(make_v(-w,  h,  d, 0.0f, 1.0f));
        vertices.push_back(make_v( w,  h, -d, 1.0f, 0.0f));
        vertices.push_back(make_v( w,  h, -d, 1.0f, 0.0f));
        vertices.push_back(make_v(-w,  h,  d, 0.0f, 1.0f));
        vertices.push_back(make_v( w,  h,  d, 1.0f, 1.0f));

        // FACE 6: BOTTOM (Facing negative Y)
        vertices.push_back(make_v(-w, -h,  d, 0.0f, 0.0f));
        vertices.push_back(make_v(-w, -h, -d, 0.0f, 1.0f));
        vertices.push_back(make_v( w, -h,  d, 1.0f, 0.0f));
        vertices.push_back(make_v( w, -h,  d, 1.0f, 0.0f));
        vertices.push_back(make_v(-w, -h, -d, 0.0f, 1.0f));
        vertices.push_back(make_v( w, -h, -d, 1.0f, 1.0f));

        return vertices;
    }
}