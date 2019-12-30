#include "mesh_loader.h"

namespace Engine {

static unsigned int StaticMeshLoader::loadStaticMesh(std::string filePath) {
    for(size_t i = 0; i < loadedStaticMeshes.size(); i++) {
        if(loadedStaticMeshes[i].filePath == filePath) {
            loadedStaticMeshes[i].count++;
            return loadedStaticMeshes[i].vaoID;
        }
    }
    unsigned int vaoID;
    // Load mesh from file system into graphics memory
    
    StaticMeshInfo staticMeshInfo;
    staticMeshInfo.filePath = filePath;
    staticMeshInfo.vaoID = vaoID;
    staticMeshInfo.count = 0;
    loadedStaticMeshes.push_back(staticMeshInfo);
    
    return vaoID;
}

static void StaticMeshLoader::releaseStaticMesh(std::string filePath) {
    for(size_t i = 0; i < loadedStaticMeshes.size(); i++) {
        if(loadedStaticMeshes[i].filePath == filePath) {
            loadedStaticMeshes[i].count--;
            break;
        }
    }
}

static void StaticMeshLoader::unloadUnusedStaticMeshes() {
    for(std::vector<StaticMeshInfo>::iterator iter = loadedStaticMeshes.begin(); iter != loadedStaticMeshes.end(); iter++) {
        if(iter->count <= 0) {
            // Unload mesh from graphics memory at vaoID
            
            loadedStaticMeshes.erase(iter);
        }
    }
}

}
