#include "mesh.h"

namespace Engine {

/*
 * Class Mesh
 */
Mesh::Mesh(const MeshDataPtr meshDataPtr, const TexturedMaterial texturedMaterial, const UnTexturedMaterial unTexturedMaterial, const std::string modelFilePath)
    : texturedMaterial(texturedMaterial), unTexturedMaterial(unTexturedMaterial) {
    this->meshID = MeshLoader::LoadMeshFromMeshData(meshDataPtr);
    MeshLoader::UseLoadedMesh(this->meshID);
}

Mesh::Mesh(const Mesh& mesh) : texturedMaterial(mesh.texturedMaterial), unTexturedMaterial(mesh.unTexturedMaterial) {
    this->meshID = mesh.meshID;
    MeshLoader::UseLoadedMesh(this->meshID);
}

Mesh::~Mesh() {
    MeshLoader::ReleaseLoadedMesh(this->meshID);
}

void Mesh::render() const {
    
}

MeshDataPtr Mesh::getMeshDataPtr() const {
    return MeshLoader::GetMeshDataPtr(this->meshID);
}

MeshDataPtr Mesh::copyMeshData() const {
    return MeshLoader::CopyMeshDataFromLoaded(this->meshID);
}

}
