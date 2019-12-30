#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <graphics/model/model.h>
#include <graphics/mesh/mesh.h>
#include <string>
#include <vector>

namespace Engine {

class StaticMeshLoader {
    public:
        static unsigned int loadStaticMesh(std::string filePath);
        static void releaseStaticMesh(std::string filePath);
        static void unloadUnusedStaticMeshes();
    private:
        struct StaticMeshInfo {
            std::string filePath;
            unsigned int vaoID;
            unsigned int count;
        };
        static std::vector<StaticMeshInfo> loadedStaticMeshes;
};

}

#endif //MESH_LOADER_H
