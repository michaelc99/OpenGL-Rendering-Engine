#ifndef MODEL_H
#define MODEL_H

#include <graphics/mesh/mesh.h>
#include <string>
#include <vector>

namespace Engine {

class ModelData;
typedef std::shared_ptr<ModelData> ModelDataPtr;
class ModelLoader;

class Model {
    public:
        Model(const std::string filePath);
        Model(const Model& model);
        
        void addMesh(Mesh mesh);
    private:
        std::string filePath;
        std::vector<Mesh> meshes;
//        std::vector<Bone> bones; // Animation?????
};

class ModelData {
    public:
        ModelData();
        void addMeshData(MeshData mesh);
        
        void formatData();
    private:
        std::vector<MeshDataPtr> meshDataList;
//        std::vector<BoneDataPtr> boneDataList; // Animation?
};

/*
 * ModelLoader handles buffering all meshes for a model defined in a model file into OpenGL along with buffering all textures referenced
 * in the model file into OpenGL. Currently buffered meshes and textures will be used instead of reloading from file system.
 */
class ModelLoader {
    friend Model;
    private:
        /*
         * 
         * If a model with the same filename is already buffered, then the buffered copy will be used. Textures or meshes that are not currently
         * buffered will be loaded from the file system.
         */
        static unsigned int loadModelFromFile(std::string filePath);
        
        struct ModelInfo {
            Model model;//??????
            unsigned int count;
        };
        static std::vector<ModelInfo> loadedModels;
};

}

#endif //MODEL_H
