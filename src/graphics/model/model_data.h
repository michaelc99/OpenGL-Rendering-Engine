#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include <graphics/mesh/mesh.h>
#include <graphics/texture/texture.h>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

namespace Engine {

class ModelData {
    public:
        /*
         * Shallow copies.
         */
        ModelData(std::vector<Mesh> meshes);

//        ModelData(std::vector<Mesh> meshes, std::vector<BoneDataPtr> boneDataList);
        
        /*
         * 
         */
        ModelData(const ModelData& modelData);
        
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
        static ModelDataPtr GetModelDataPtr(const unsigned int modelID);
        
        /*
         * Loads model from file system into system memory. Returns the index of model from list of loaded models.
         * If a model with the same filename is already loaded, then the loaded instance will be used. If the model is
         * not currently loaded then it will be loaded from the file system.
         */
        static unsigned int LoadModelFromFile(const std::string modelFilePath);
        
        /*
         * Puts model with data given by ModelDataPtr into list of loaded model. Returns the index of the model
         * from list of loaded models.
         */
        static unsigned int LoadModelFromModelData(const ModelDataPtr modelDataPtr);
        
        /*
         * Saves modelData to a ".modeldat" file in file system.
         */
        static void SaveModelFromModelData(const std::string& modelFilePath, const ModelDataPtr modelDataPtr);
        
        /*
         * Increments using count for loaded model with index modelID from list of loaded models and ensures it is
         * buffered with OpenGL.
         */
        static void UseLoadedModel(const unsigned int modelID);
        
        /*
         * Decrements using count for model with index modelID from list of loaded models.
         */
        static void ReleaseLoadedModel(const unsigned int modelID);
        
        /*
         * Returns a deep copy of the loaded model data with index modelID from list of loaded models.
         */
        static ModelDataPtr CopyModelDataFromLoaded(const unsigned int modelID);
    private:
        /*
         * Unloads loaded model from system memory. Removes the model from list of loaded models.
         */
        static void UnloadModel(const unsigned int modelID);
        
        struct ModelInfo {
            std::string modelFilePath;
            ModelDataPtr modelDataPtr;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static unsigned int spareID;
        static std::stack<unsigned int> availableIDStack;
        static std::unordered_map<unsigned int, ModelInfo> loadedModels;
};

}

#endif //MODEL_DATA_H
