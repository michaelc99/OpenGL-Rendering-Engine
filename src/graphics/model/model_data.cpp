#include "model_data.h"

namespace Engine {

/*
 * Class ModelData
 */
ModelData::ModelData() {}

ModelData::ModelData(std::vector<Mesh> meshes) : meshes(meshes) {}

ModelData::ModelData(const ModelData& modelData) : meshes(modelData.meshes) {}

/*
 * Class ModelLoader
 */
void ModelLoader::PreLoadModels(const std::vector<std::string>& modelFilePaths) {
    
}

void ModelLoader::UnloadUnusedModels() {
    for(int i = 0; i < loadedModels.size(); i++) {
        if(loadedModels[i].usingCount == 0) {
            UnloadModel(i);
        }
    }
}

ModelDataPtr ModelLoader::GetModelDataPtr(const int modelID) {
#ifdef _DEBUG
    assert(modelID > -1);
    assert(modelID < loadedMeshes.size());
#endif
    return loadedModels[modelID].modelDataPtr;
}

int ModelLoader::LoadModelFromFile(const std::string filePath) {
    
}

int ModelLoader::LoadModelFromModelData(const ModelDataPtr modelDataPtr) {
    
}

void ModelLoader::UseLoadedModel(const int modelID) {
#ifdef _DEBUG
    assert(modelID > -1);
    assert(modelID < loadedModels.size());
#endif
    loadedModels[modelID].usingCount++;
}

void ModelLoader::ReleaseLoadedModel(const int modelID) {
#ifdef _DEBUG
    assert(modelID > -1);
    assert(modelID < loadedModels.size());
    assert(loadedModels[modelID].usingCount > 0);
#endif
    loadedModels[modelID].modelDataPtr--;
}

ModelDataPtr ModelLoader::CopyModelDataFromLoaded(const int modelID) {
#ifdef _DEBUG
    assert(modelID > -1);
    assert(modelID < loadedModels.size());
#endif
    return std::make_shared<ModelData>(*(loadedModels[modelID].modelDataPtr));
}

void ModelLoader::UnloadModel(const int modelID) {
#ifdef _DEBUG
    assert(modelID > -1);
    assert(modelID < loadedModels.size());
#endif
    int index = 0;
    for(std::vector<ModelInfo>::iterator iter = loadedModels.begin(); iter != loadedModels.end(); iter++) {
        if(index == modelID) {
            loadedModels.erase(iter);
            break;
        }
        index++;
    }
}

}
