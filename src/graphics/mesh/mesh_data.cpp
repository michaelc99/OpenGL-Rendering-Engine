#include "mesh_data.h"

namespace Engine {

/*
 * Class MeshData
 */
MeshData::MeshData(const VectorPtr<Math::Vec3ui> indices, const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath) {
    this->meshGeometryID = MeshGeometryLoader::LoadMeshFromMeshGeometryData(meshGeometryDataPtr, modelFilePath);
    MeshGeometryLoader::UseLoadedMeshGeometry(this->meshGeometryID);
    this->indices = indices;
}

MeshData::MeshData(const MeshData& meshData) {
    this->meshGeometryID = meshData.meshGeometryID;
    this->indices = std::make_shared<std::vector<Math::Vec3ui>>();
    for(unsigned int i = 0; i < meshData.indices->size(); i++) {
        (*(this->indices))[i] = (*(meshData.indices))[i];
    }
    MeshGeometryLoader::UseLoadedMeshGeometry(this->meshGeometryID);
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
std::vector<MeshLoader::MeshInfo> MeshLoader::loadedMeshes = std::vector<MeshLoader::MeshInfo>();

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
    glBindVertexArray(loadedMeshes[meshID].meshVAO);
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
    MeshGeometryLoader::RequireMeshGeometryBuffered(loadedMeshes[meshID].meshDataPtr->getMeshGeometryID());
    
    glGenVertexArrays(1, &(loadedMeshes[meshID].meshVAO));
    glBindVertexArray(loadedMeshes[meshID].meshVAO);
    MeshGeometryLoader::BindMeshGeometry(loadedMeshes[meshID].meshDataPtr->getMeshGeometryID());
    
    glGenBuffers(1, &(loadedMeshes[meshID].meshEBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedMeshes[meshID].meshEBO);
    unsigned int indexStride = 3;
    unsigned int totalNumValues = loadedMeshes[meshID].meshDataPtr->getIndices()->size() * indexStride;
    std::unique_ptr<unsigned int[]> indexData = std::unique_ptr<unsigned int[]>(new unsigned int[totalNumValues]);
    for(unsigned int i = 0; i < loadedMeshes[meshID].meshDataPtr->getIndices()->size(); i++) {
        for(unsigned int j = 0; j < indexStride; j++) {
            indexData.get()[i * indexStride + j] = (*(loadedMeshes[meshID].meshDataPtr->getIndices()))[i][j];
        }
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalNumValues * sizeof(unsigned int), indexData.get(), GL_STATIC_DRAW);
    
    unsigned int vertexStride = 3 * sizeof(float);
    unsigned int normalStride = 3 * sizeof(float);
    unsigned int textureCoordStride = 2 * sizeof(float);
    unsigned int stride = vertexStride + normalStride + textureCoordStride;
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(size_t)0);
    glEnableVertexAttribArray(0);
    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(size_t)(vertexStride));
    glEnableVertexAttribArray(1);
    // Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(size_t)(vertexStride + normalStride));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshLoader::UnBufferMeshData(const int meshID) {
    glDeleteVertexArrays(1, &(loadedMeshes[meshID].meshVAO));
    glDeleteBuffers(1, &(loadedMeshes[meshID].meshEBO));
    MeshGeometryLoader::RelaxMeshGeometryBuffered(loadedMeshes[meshID].meshDataPtr->getMeshGeometryID());
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
