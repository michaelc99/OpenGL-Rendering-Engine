#include "model.h"

namespace Engine {

/*
 * Class Model
 */
Model::Model(const std::string modelFilePath) {
    this->modelID = ModelLoader::LoadModelFromFile(modelFilePath);
    ModelLoader::UseLoadedModel(this->modelID);
}

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
    
}

MeshDataPtr Mesh::getMeshDataPtr() const {
    return MeshLoader::GetMeshDataPtr(this->meshID);
}

MeshDataPtr Mesh::copyMeshData() const {
    return MeshLoader::CopyMeshDataFromLoaded(this->meshID);
}

}
