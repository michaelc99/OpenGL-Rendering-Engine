#include "mesh_data.h"

namespace Engine {

/*
 * Class MeshData
 */
MeshData::MeshData() {}

MeshData::MeshData(const VectorPtr<Math::Vec3f> vertices, const VectorPtr<Math::Vec3f> normals, const VectorPtr<Math::Vec2f> textureCoords,
        const VectorPtr<Math::Vec3ui> indices) {
#ifdef _DEBUG
    unsigned int size = vertices.size();
    assert(normals.size() == size);
    assert(textureCoords.size() == size);
    assert(indices.size() == size);
#endif
    this->vertices = vertices;
    this->normals = normals;
    this->textureCoords = textureCoords;
    this->indices = indices;
}

MeshData::MeshData(const MeshData& meshData) {
#ifdef _DEBUG
    unsigned int size = meshData.vertices.size();
    assert(meshData.normals.size() == size);
    assert(meshData.textureCoords.size() == size);
    assert(meshData.indices.size() == size);
#endif
    this->vertices = std::make_shared<std::vector<Math::Vec3f>>();
    for(unsigned int i = 0; i < meshData.vertices->size(); i++) {
        (*(this->vertices))[i] = (*(meshData.vertices))[i];
    }
    this->normals = std::make_shared<std::vector<Math::Vec3f>>();
    for(unsigned int i = 0; i < meshData.normals->size(); i++) {
        (*(this->normals))[i] = (*(meshData.normals))[i];
    }
    this->textureCoords = std::make_shared<std::vector<Math::Vec2f>>();
    for(unsigned int i = 0; i < meshData.textureCoords->size(); i++) {
        (*(this->textureCoords))[i] = (*(meshData.textureCoords))[i];
    }
    this->indices = std::make_shared<std::vector<Math::Vec3ui>>();
    for(unsigned int i = 0; i < meshData.indices->size(); i++) {
        (*(this->indices))[i] = (*(meshData.indices))[i];
    }
}

/*
 * Class MeshLoader
 */
void MeshLoader::UnloadUnusedMeshes() {
    for(int i = 0; i < loadedMeshes.size(); i++) {
        if(loadedMeshes[i].usingCount == 0) {
            UnloadMesh(i);
        }
    }
}

MeshDataPtr MeshLoader::GetMeshDataPtr(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < loadedMeshes.size());
#endif
    return loadedMeshes[meshID].meshDataPtr;
}

void MeshLoader::BindMesh(const int meshID) {
    
}

int MeshLoader::LoadMeshFromMeshData(const MeshDataPtr meshDataPtr) {
    MeshInfo meshInfo;
    meshInfo.meshDataPtr = meshDataPtr;
    meshInfo.meshVAO = 0;
    meshInfo.usingCount = 0;
    loadedMeshes.push_back(meshInfo);
    
    int meshID = loadedMeshes.size() - 1;
    return meshID;
}

void MeshLoader::UseLoadedMesh(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < loadedMeshes.size());
#endif
    if(loadedMeshes[meshID].usingCount == 0) {
        BufferMeshData(meshID);
    }
    loadedMeshes[meshID].usingCount++;
}

void MeshLoader::ReleaseLoadedMesh(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < loadedMeshes.size());
    assert(loadedMeshes[meshID].usingCount > 0);
#endif
    loadedMeshes[meshID].usingCount--;
    if(loadedMeshes[meshID].usingCount == 0) {
        UnBufferMeshData(meshID);
    }
}

MeshDataPtr MeshLoader::CopyMeshDataFromLoaded(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < loadedMeshes.size());
#endif
    return std::make_shared<MeshData>(*(loadedMeshes[meshID].meshDataPtr));
}

void MeshLoader::BufferMeshData(const int meshID) {
    
}

void MeshLoader::UnBufferMeshData(const int meshID) {
    
}

void MeshLoader::UnloadMesh(const int meshID) {
#ifdef _DEBUG
    assert(meshID > -1);
    assert(meshID < loadedMeshes.size());
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
