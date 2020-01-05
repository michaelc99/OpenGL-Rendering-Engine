#include "model_data.h"

namespace Engine {

/*
 * Class ModelData
 */
ModelData::ModelData(std::vector<Mesh> meshes) : meshes(meshes) {}

ModelData::ModelData(const ModelData& modelData) : meshes(modelData.meshes) {}

/*
 * Class ModelLoader
 */
unsigned int ModelLoader::spareID = 1;
std::stack<unsigned int> ModelLoader::availableIDStack = std::stack<unsigned int>();
std::unordered_map<unsigned int, ModelLoader::ModelInfo> ModelLoader::loadedModels = std::unordered_map<unsigned int, ModelLoader::ModelInfo>();

//void ModelLoader::PreLoadModels(const std::vector<std::string>& modelFilePaths) {
//    
//}

void ModelLoader::UnloadUnusedModels() {
    for(std::unordered_map<unsigned int, ModelInfo>::iterator iter = loadedModels.begin(); iter != loadedModels.end(); iter++) {
        if(iter->second.usingCount == 0) {
            UnloadModel(iter->first);
        }
    }
}

ModelDataPtr ModelLoader::GetModelDataPtr(const unsigned int modelID) {
#ifdef _DEBUG
    assert(modelID != 0 && modelID < spareID);
#endif
    return loadedModels[modelID].modelDataPtr;
}

//unsigned int ModelLoader::LoadModelFromFile(const std::string filePath) {
//    
//}

unsigned int ModelLoader::LoadModelFromModelData(const ModelDataPtr modelDataPtr) {
    ModelInfo modelInfo;
    modelInfo.modelFilePath = "";
    modelInfo.modelDataPtr = std::make_shared<ModelData>(*(modelDataPtr.get()));
    modelInfo.usingCount = 0;
    if(availableIDStack.empty()) {
        availableIDStack.push(spareID++);
    }
    unsigned int modelID = availableIDStack.top();
    availableIDStack.pop();
    loadedModels[modelID] = modelInfo;
    return modelID;
}

//void ModelLoader::SaveModelFromModelData(const std::string& filePath, const ModelDataPtr modelDataPtr) {
//    
//}

void ModelLoader::UseLoadedModel(const unsigned int modelID) {
#ifdef _DEBUG
    assert(modelID != 0 && modelID < spareID);
#endif
    loadedModels[modelID].usingCount++;
}

void ModelLoader::ReleaseLoadedModel(const unsigned int modelID) {
#ifdef _DEBUG
    assert(modelID != 0 && modelID < spareID);
    assert(loadedModels[modelID].usingCount > 0);
#endif
    loadedModels[modelID].usingCount--;
    if(loadedModels[modelID].usingCount == 0) {
        if(loadedModels[modelID].modelFilePath == "") {
            UnloadModel(modelID);
        }
    }
}

ModelDataPtr ModelLoader::CopyModelDataFromLoaded(const unsigned int modelID) {
#ifdef _DEBUG
    assert(modelID != 0 && modelID < spareID);
#endif
    return std::make_shared<ModelData>(*(loadedModels[modelID].modelDataPtr));
}

void ModelLoader::UnloadModel(const unsigned int modelID) {
#ifdef _DEBUG
    assert(modelID != 0 && modelID < spareID);
#endif
    for(std::unordered_map<unsigned int, ModelInfo>::iterator iter = loadedModels.begin(); iter != loadedModels.end(); iter++) {
        if(iter->first == modelID) {
            availableIDStack.push(iter->first);
            loadedModels.erase(iter);
            break;
        }
    }
}

}
