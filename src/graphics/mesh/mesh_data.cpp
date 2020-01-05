#include "mesh_data.h"

namespace Engine {

/*
 * Class MeshData
 */
MeshData::MeshData(const VectorPtr<unsigned int> indices, const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath) {
    this->meshGeometryID = MeshGeometryLoader::LoadMeshFromMeshGeometryData(meshGeometryDataPtr, modelFilePath);
    MeshGeometryLoader::UseLoadedMeshGeometry(this->meshGeometryID);
    this->indices = indices;
}

MeshData::MeshData(const MeshData& meshData) {
    this->meshGeometryID = meshData.meshGeometryID;
    MeshGeometryLoader::UseLoadedMeshGeometry(this->meshGeometryID);
    this->indices = std::make_shared<std::vector<unsigned int>>();
    for(unsigned int i = 0; i < meshData.indices->size(); i++) {
        this->indices->push_back((*(meshData.indices))[i]);
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
unsigned int MeshLoader::spareID = 1;
std::stack<unsigned int> MeshLoader::availableIDStack = std::stack<unsigned int>();
std::unordered_map<unsigned int, MeshLoader::MeshInfo> MeshLoader::loadedMeshes = std::unordered_map<unsigned int, MeshLoader::MeshInfo>();

void MeshLoader::UnloadUnusedMeshes() {
    for(unsigned int i = 0; i < loadedMeshes.size(); i++) {
        if(loadedMeshes[i].usingCount == 0) {
            UnloadMesh(i);
        }
    }
}

MeshDataPtr MeshLoader::GetMeshDataPtr(const unsigned int meshID) {
#ifdef _DEBUG
    assert(meshID != 0 && meshID < spareID);
#endif
    return loadedMeshes[meshID].meshDataPtr;
}

void MeshLoader::BindMesh(const unsigned int meshID) {
    glBindVertexArray(loadedMeshes[meshID].meshVAO);
}

unsigned int MeshLoader::LoadMeshFromMeshData(const MeshDataPtr meshDataPtr, const std::string modelFilePath) {
    MeshInfo meshInfo;
    meshInfo.modelFilePath = modelFilePath;
    meshInfo.meshDataPtr = std::make_shared<MeshData>(*(meshDataPtr.get()));
    meshInfo.meshEBO = 0;
    meshInfo.meshVAO = 0;
    meshInfo.usingCount = 0;
    if(availableIDStack.empty()) {
        availableIDStack.push(spareID++);
    }
    unsigned int meshID = availableIDStack.top();
    availableIDStack.pop();
    loadedMeshes[meshID] = meshInfo;
    return meshID;
}

void MeshLoader::UseLoadedMesh(const unsigned int meshID) {
#ifdef _DEBUG
    assert(meshID != 0 && meshID < spareID);
#endif
    if(loadedMeshes[meshID].usingCount == 0) {
        BufferMeshData(meshID);
    }
    loadedMeshes[meshID].usingCount++;
    std::cout << "UseLoadedMesh ID = " << meshID << ", new count = " << loadedMeshes[meshID].usingCount << "\n";
}

void MeshLoader::ReleaseLoadedMesh(const unsigned int meshID) {
#ifdef _DEBUG
    assert(meshID != 0 && meshID < spareID);
    assert(loadedMeshes[meshID].usingCount > 0);
#endif
    loadedMeshes[meshID].usingCount--;
    std::cout << "ReleaseLoadedMesh ID = " << meshID << ", new count = " << loadedMeshes[meshID].usingCount << "\n";
    if(loadedMeshes[meshID].usingCount == 0) {
        UnBufferMeshData(meshID);
        if(loadedMeshes[meshID].modelFilePath == "") {
            UnloadMesh(meshID);
        }
    }
}

MeshDataPtr MeshLoader::CopyMeshDataFromLoaded(const unsigned int meshID) {
#ifdef _DEBUG
    assert(meshID != 0 && meshID < spareID);
#endif
    MeshGeometryDataPtr meshGeometryDataPtr = loadedMeshes[meshID].meshDataPtr->copyMeshGeometryData();
    MeshDataPtr meshDataPtr = std::make_shared<MeshData>(*(loadedMeshes[meshID].meshDataPtr));
    meshDataPtr->setMeshGeometryDataPtr(meshGeometryDataPtr);
    return meshDataPtr;
}

void MeshLoader::BufferMeshData(const unsigned int meshID) {
    MeshGeometryLoader::RequireMeshGeometryBuffered(loadedMeshes[meshID].meshDataPtr->getMeshGeometryID());
    
    glGenVertexArrays(1, &(loadedMeshes[meshID].meshVAO));
    glBindVertexArray(loadedMeshes[meshID].meshVAO);
    MeshGeometryLoader::BindMeshGeometry(loadedMeshes[meshID].meshDataPtr->getMeshGeometryID());
    
    glGenBuffers(1, &(loadedMeshes[meshID].meshEBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedMeshes[meshID].meshEBO);
    unsigned int indexStride = 1;
    unsigned int totalNumValues = loadedMeshes[meshID].meshDataPtr->getIndices()->size() * indexStride;
    std::unique_ptr<unsigned int[]> indexData = std::unique_ptr<unsigned int[]>(new unsigned int[totalNumValues]);
    for(unsigned int i = 0; i < loadedMeshes[meshID].meshDataPtr->getIndices()->size(); i++) {
        indexData.get()[i] = (*(loadedMeshes[meshID].meshDataPtr->getIndices()))[i];
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

void MeshLoader::UnBufferMeshData(const unsigned int meshID) {
    glDeleteVertexArrays(1, &(loadedMeshes[meshID].meshVAO));
    glDeleteBuffers(1, &(loadedMeshes[meshID].meshEBO));
    std::cout << "unbuffering\n";
    MeshGeometryLoader::RelaxMeshGeometryBuffered(loadedMeshes[meshID].meshDataPtr->getMeshGeometryID());
}

void MeshLoader::UnloadMesh(const unsigned int meshID) {
#ifdef _DEBUG
    assert(meshID != 0 && meshID < spareID);
    assert(loadedMeshes[meshID].usingCount == 0);
#endif
    for(std::unordered_map<unsigned int, MeshInfo>::iterator iter = loadedMeshes.begin(); iter != loadedMeshes.end(); iter++) {
        if(iter->first == meshID) {
            availableIDStack.push(iter->first);
            loadedMeshes.erase(iter);
            break;
        }
    }
}

}
