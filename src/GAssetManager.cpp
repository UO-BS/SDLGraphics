#include <GEngine/GAssetManager.h>

void GAssetManager::init() {
    textureSubsystem.init();
    
    // There is no init function for the mesh subsystem
    // meshSubsystem.init(); 
}

void GAssetManager::clearAll() {
    // Meshes cleared first since they point to textures
    meshSubsystem.clear();
    
    // Clear all textures (except fallback texture)
    textureSubsystem.clear();
}

const GTexture* GAssetManager::loadTexture(const std::string& textureFilePath) {
    return textureSubsystem.findOrLoadTexture(textureFilePath);
}

const GMesh* GAssetManager::loadMesh(const std::string& meshName, const std::vector<GTextureVertex>& vertices, const GTexture* texture) 
{
    return meshSubsystem.getOrRegisterMesh(meshName, vertices, texture);
}