#include "model.h"

namespace Engine {

/*
 * Class Mesh
 */
Model::Model(const std::string filePath) {
    this->modelID = ModelLoader::LoadModelFromFile(filePath);
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
