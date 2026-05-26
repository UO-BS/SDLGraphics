#ifndef G_ASSET_MANAGER_H
#define G_ASSET_MANAGER_H

#include <GEngine/GTextureManager.h>
#include <GEngine/GMeshManager.h>

// Facade Pattern to hold all the textures, meshes, etc... for a given scene

class GAssetManager {
public:
    GAssetManager() = default;
    ~GAssetManager() = default;

    void init();

    // Wipe all loaded assets (except the fallback texture)
    void clearAll();

    // Texture Manager
    // Will return the fallback texture if it cannot find the desired texture
    const GTexture* loadTexture(const std::string& textureFilePath);

    // Mesh Manager
    // Will return nullptr if it cannot find the desired mesh (and no new mesh is being created)
    const GMesh* loadMesh(const std::string& name, const std::vector<GTextureVertex>& vertices, const GTexture* texture);

    // No Copying
    GAssetManager(const GAssetManager&) = delete;
    GAssetManager& operator=(const GAssetManager&) = delete;
    
private:

    GTextureManager textureSubsystem;
    GMeshManager meshSubsystem;
};

#endif