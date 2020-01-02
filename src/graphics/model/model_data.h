#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include <graphics/mesh/mesh.h>
#include <graphics/texture/texture.h>
#include <string>
#include <vector>

namespace Engine {

class ModelData {
    public:
        ModelData();
        ModelData(std::vector<Mesh> meshes);
//        ModelData(std::vector<Mesh> meshes, std::vector<BoneDataPtr> boneDataList);
        ModelData(const MeshData& meshData);
        
        std::vector<Mesh> getMeshes() const { return meshes; }
        void setMeshes(const std::vector<Mesh> meshes) { this->meshes = meshes; }
    private:
        std::vector<Mesh> meshes;
//        std::vector<BoneDataPtr> boneDataList; // Animation?????
};
typedef std::shared_ptr<ModelData> ModelDataPtr;

/*
 * ModelLoader handles buffering all meshes for a model defined in a model file into OpenGL along with buffering all textures referenced
 * in the model file into OpenGL. Currently buffered meshes and textures will be used instead of reloading from file system.
 */
class ModelLoader {
    public:
        /*
         * Loads models from list of file paths into system memory.
         */
        static void PreLoadModels(const std::vector<std::string>& modelFilePaths);
        
        /*
         * Unloads all loaded models with usage count less than 1 from system memory.
         */
        static void UnloadUnusedModels();
        
        /*
         * Returns a ModelData pointer to model data with index modelID from list of loaded models.
         */
        static ModelDataPtr GetModelDataPtr(const int modelID);
        
        /*
         * Loads model from file system into system memory. Returns the index of model from list of loaded models.
         * If a model with the same filename is already loaded, then the loaded instance will be used. If the model is
         * not currently loaded then it will be loaded from the file system.
         */
        static int LoadModelFromFile(const std::string filePath);
        
        /*
         * Puts model with data given by ModelDataPtr into list of loaded model. Returns the index of the model
         * from list of loaded models.
         */
        static int LoadModelFromModelData(const ModelDataPtr modelDataPtr);
        
        /*
         * Increments using count for loaded model with index modelID from list of loaded models and ensures it is
         * buffered with OpenGL.
         */
        static void UseLoadedModel(const int modelID);
        
        /*
         * Decrements using count for model with index modelID from list of loaded models.
         */
        static void ReleaseLoadedModel(const int modelID);
        
        /*
         * Returns a deep copy of the loaded model data with index modelID from list of loaded models.
         */
        static ModelDataPtr CopyModelDataFromLoaded(const int modelID);
    private:
        /*
         * Unloads loaded model from system memory. Removes the model from list of loaded models.
         */
        static void UnloadModel(const int modelID);
        
        struct ModelInfo {
            std::string filePath;
            ModelDataPtr modelDataPtr;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<ModelInfo> loadedModels;
};

}

#endif //MODEL_DATA_H
