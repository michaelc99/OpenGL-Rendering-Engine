#include "mesh_data.h"

namespace Engine {

/*
 * Class MeshData
 */
MeshData::MeshData(const VectorPtr<Math::Vec3ui> indices, const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath) {
    this->meshGeometryID = MeshGeometryLoader::LoadMeshFromMeshGeometryData(meshGeometryDataPtr, modelFilePath);
    this->indices = indices;
}

MeshData::MeshData(const MeshData& meshData) {
    this->meshGeometryID = meshData.meshGeometryID;
    this->indices = std::make_shared<std::vector<Math::Vec3ui>>();
    for(unsigned int i = 0; i < meshData.indices->size(); i++) {
        (*(this->indices))[i] = (*(meshData.indices))[i];
    }
}

MeshData::~MeshData() {
    MeshGeometryLoader::ReleaseLoadedMeshGeometry(this->meshGeometryID);
}

MeshGeometryDataPtr MeshData::getMeshGeometryDataPtr() const {
    return MeshGeometryLoader::GetMeshGeometryDataPtr(this->meshGeometryID);
}

void MeshData::setMeshGeometryDataPtr(const MeshGeometryDataPtr meshGeometryDataPtr) {
    MeshGeometryLoader::ReleaseLoadedMeshGeometry(this->meshGeometryID);
    this->meshGeometryID = MeshGeometryLoader::LoadMeshFromMeshGeometryData(meshGeometryDataPtr);
}
        
MeshGeometryDataPtr MeshData::copyMeshGeometryData() const {
    return MeshGeometryLoader::CopyMeshGeometryDataFromLoaded(this->meshGeometryID);
}

/*
 * Class MeshLoader
 */
void MeshLoader::UnloadUnusedMeshes() {
    for(unsigned int i = 0; i < loadedMeshes.size(); i++) {
        if(loadedMeshes[i].usingCount == 0) {
            UnloadMesh(i);
        }
    }
}

MeshDataPtr MeshLoader::GetMeshDataPtr(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < (int)loadedMeshes.size());
#endif
    return loadedMeshes[meshID].meshDataPtr;
}

void MeshLoader::BindMesh(const int meshID) {
    
}

int MeshLoader::LoadMeshFromMeshData(const MeshDataPtr meshDataPtr, const std::string modelFilePath) {
    MeshInfo meshInfo;
    meshInfo.modelFilePath = modelFilePath;
    meshInfo.meshDataPtr = meshDataPtr;
    meshInfo.meshEBO = 0;
    meshInfo.meshVAO = 0;
    meshInfo.usingCount = 0;
    loadedMeshes.push_back(meshInfo);
    
    int meshID = loadedMeshes.size() - 1;
    return meshID;
}

void MeshLoader::UseLoadedMesh(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < (int)loadedMeshes.size());
#endif
    if(loadedMeshes[meshID].usingCount == 0) {
        BufferMeshData(meshID);
    }
    loadedMeshes[meshID].usingCount++;
}

void MeshLoader::ReleaseLoadedMesh(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < (int)loadedMeshes.size());
    assert(loadedMeshes[meshID].usingCount > 0);
#endif
    loadedMeshes[meshID].usingCount--;
    if(loadedMeshes[meshID].usingCount == 0) {
        UnBufferMeshData(meshID);
        if(loadedMeshes[meshID].modelFilePath == "") {
            UnloadMesh(meshID);
        }
    }
}

MeshDataPtr MeshLoader::CopyMeshDataFromLoaded(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < (int)loadedMeshes.size());
#endif
    MeshGeometryDataPtr meshGeometryDataPtr = loadedMeshes[meshID].meshDataPtr->copyMeshGeometryData();
    MeshDataPtr meshDataPtr = std::make_shared<MeshData>(*(loadedMeshes[meshID].meshDataPtr));
    meshDataPtr->setMeshGeometryDataPtr(meshGeometryDataPtr);
    return meshDataPtr;
}

void MeshLoader::BufferMeshData(const int meshID) {
    
}

void MeshLoader::UnBufferMeshData(const int meshID) {
    
}

void MeshLoader::UnloadMesh(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < (int)loadedMeshes.size());
    assert(loadedMeshes[meshID].usingCount == 0);
#endif
    int index = 0;
    for(std::vector<MeshInfo>::iterator iter = loadedMeshes.begin(); iter != loadedMeshes.end(); iter++) {
        if(index == meshID) {
            loadedMeshes.erase(iter);
            break;
        }
        index++;
    }
}

}
