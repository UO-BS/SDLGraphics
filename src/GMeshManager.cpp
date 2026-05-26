#include "GEngine/GMeshManager.h"

void GMeshManager::clear() {
    // Clearing the objects from the map will automatically free the texture. 
    //      The map holds a unique pointer to the GTexture struct, which has a destructor that frees the SDL_Texture.
    meshMap.clear();
}

const GMesh* GMeshManager::getOrRegisterMesh(const std::string& meshName, std::vector<GTextureVertex> vertices, const GTexture* texture) {
    // Find mesh
    auto it = meshMap.find(meshName);
    if (it != meshMap.end()) {
        return it->second.get(); 
    }

    // Mesh does not exist - Create it
    
    // User tried to find a Mesh, didn't find it, and did not leave any data to register
    if (vertices.empty() || texture == nullptr) {
        return nullptr;
    }

    // Create a GMesh object
    auto newMesh = std::make_unique<GMesh>(vertices, texture);
    // Get pointer first, so that we dont have to search the map after moving
    const GMesh* meshPtr = newMesh.get();
    
    meshMap[meshName] = std::move(newMesh);
    return meshPtr;
}
