#include "model.h"

namespace Engine {

/*
 * Class Model
 */
//Model::Model(const std::string modelFilePath) {
//    this->modelID = ModelLoader::LoadModelFromFile(modelFilePath);
//    ModelLoader::UseLoadedModel(this->modelID);
//}

Model::Model(const ModelDataPtr modelDataPtr) {
    this->modelID = ModelLoader::LoadModelFromModelData(modelDataPtr);
    ModelLoader::UseLoadedModel(this->modelID);
}

Model::Model(const Model& model) {
    this->modelID = model.modelID;
    ModelLoader::UseLoadedModel(this->modelID);
}

Model::~Model() {
    ModelLoader::ReleaseLoadedModel(this->modelID);
}

void Model::render() const {
    std::vector<Mesh> meshes = getModelDataPtr()->getMeshes();
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].render();
    }
}

ModelDataPtr Model::getModelDataPtr() const {
    return ModelLoader::GetModelDataPtr(this->modelID);
}

ModelDataPtr Model::copyModelData() const {
    return ModelLoader::CopyModelDataFromLoaded(this->modelID);
}

}
