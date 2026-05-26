#ifndef G_MESH_MANAGER_H
#define G_MESH_MANAGER_H

#include <vector>
#include <unordered_map>
#include "GCommonStructs.h"
#include <memory>
#include "GMesh.h"

/*
    Manage loaded Meshes
    Holds unique_ptrs to all GMesh objects
*/

class GMeshManager { 
public:

    void clear();

    // Returns a nullptr if the mesh was not found, and no vertices/texture exist to register a new mesh
    const GMesh* getOrRegisterMesh(const std::string& meshName, std::vector<GTextureVertex> vertices, const GTexture* texture);

private:

    std::unordered_map<std::string, std::unique_ptr<GMesh>> meshMap;

};

#endif