#include "mesh_geometry_data.h"

namespace Engine {

/*
 * Class MeshGeometryData
 */
MeshGeometryData::MeshGeometryData(const VectorPtr<Math::Vec3f> vertices, const VectorPtr<Math::Vec3f> normals, const VectorPtr<Math::Vec2f> textureCoords) {
#ifdef _DEBUG
    unsigned int size = vertices->size();
    assert(normals->size() == size);
    assert(textureCoords->size() == size);
#endif
    this->vertices = vertices;
    this->normals = normals;
    this->textureCoords = textureCoords;
}

MeshGeometryData::MeshGeometryData(const MeshGeometryData& meshGeometryData) {
#ifdef _DEBUG
    unsigned int size = meshGeometryData.vertices->size();
    assert(meshGeometryData.normals->size() == size);
    assert(meshGeometryData.textureCoords->size() == size);
#endif
    this->vertices = std::make_shared<std::vector<Math::Vec3f>>();
    for(unsigned int i = 0; i < meshGeometryData.vertices->size(); i++) {
        this->vertices->push_back((*(meshGeometryData.vertices.get()))[i]);
    }
    this->normals = std::make_shared<std::vector<Math::Vec3f>>();
    for(unsigned int i = 0; i < meshGeometryData.normals->size(); i++) {
        this->normals->push_back((*(meshGeometryData.normals.get()))[i]);
    }
    this->textureCoords = std::make_shared<std::vector<Math::Vec2f>>();
    for(unsigned int i = 0; i < meshGeometryData.textureCoords->size(); i++) {
        this->textureCoords->push_back((*(meshGeometryData.textureCoords.get()))[i]);
    }
}

/*
 * Class MeshGeometryLoader
 */
unsigned int MeshGeometryLoader::spareID = 1;
std::stack<unsigned int> MeshGeometryLoader::availableIDStack = std::stack<unsigned int>();
std::unordered_map<unsigned int, MeshGeometryLoader::MeshGeometryInfo> MeshGeometryLoader::loadedMeshGeometries = std::unordered_map<unsigned int, MeshGeometryLoader::MeshGeometryInfo>();

void MeshGeometryLoader::UnloadUnusedMeshGeometries() {
    for(std::unordered_map<unsigned int, MeshGeometryInfo>::iterator iter = loadedMeshGeometries.begin(); iter != loadedMeshGeometries.end(); iter++) {
        if(iter->second.usingCount == 0) {
            UnloadMeshGeometry(iter->first);
        }
    }
}

MeshGeometryDataPtr MeshGeometryLoader::GetMeshGeometryDataPtr(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
#endif
    return loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr;
}

void MeshGeometryLoader::BindMeshGeometry(const unsigned int meshGeometryID) {
    glBindBuffer(GL_ARRAY_BUFFER, loadedMeshGeometries[meshGeometryID].meshVBO);
}

unsigned int MeshGeometryLoader::LoadMeshFromMeshGeometryData(const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath) {
    // Check if the mesh geometry is already buffered
    if(modelFilePath != "") {
        for(std::unordered_map<unsigned int, MeshGeometryInfo>::iterator iter = loadedMeshGeometries.begin(); iter != loadedMeshGeometries.end(); iter++) {
            if(iter->second.modelFilePath == modelFilePath) {
                iter->first;
            }
        }
    }
    
    MeshGeometryInfo meshGeometryInfo;
    meshGeometryInfo.modelFilePath = modelFilePath;
    meshGeometryInfo.meshGeometryDataPtr = std::make_shared<MeshGeometryData>(*(meshGeometryDataPtr.get()));
    meshGeometryInfo.meshVBO = 0;
    meshGeometryInfo.usingCount = 0;
    if(availableIDStack.empty()) {
        availableIDStack.push(spareID++);
    }
    unsigned int meshGeometryID = availableIDStack.top();
    availableIDStack.pop();
    loadedMeshGeometries[meshGeometryID] = meshGeometryInfo;
    return meshGeometryID;
}

void MeshGeometryLoader::UseLoadedMeshGeometry(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
#endif
    loadedMeshGeometries[meshGeometryID].usingCount++;
    std::cout << "UseLoadedMeshGeometry ID = " << meshGeometryID << ", new count = " << loadedMeshGeometries[meshGeometryID].usingCount << "\n";
}

void MeshGeometryLoader::ReleaseLoadedMeshGeometry(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
    assert(loadedMeshGeometries[meshGeometryID].usingCount > 0);
#endif
    loadedMeshGeometries[meshGeometryID].usingCount--;
    std::cout << "ReleaseLoadedMeshGeometry ID = " << meshGeometryID << ", new count = " << loadedMeshGeometries[meshGeometryID].usingCount << "\n";
    if(loadedMeshGeometries[meshGeometryID].usingCount == 0) {
        UnBufferMeshGeometryData(meshGeometryID);
        if(loadedMeshGeometries[meshGeometryID].modelFilePath == "") {
            UnloadMeshGeometry(meshGeometryID);
        }
    }
}

void MeshGeometryLoader::RequireMeshGeometryBuffered(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
#endif
    if(loadedMeshGeometries[meshGeometryID].usingBufferedCount == 0) {
        BufferMeshGeometryData(meshGeometryID);
    }
    loadedMeshGeometries[meshGeometryID].usingBufferedCount++;
    std::cout << "RequireMeshGeometryBuffered ID = " << meshGeometryID << ", new count = " << loadedMeshGeometries[meshGeometryID].usingBufferedCount << "\n";
}

void MeshGeometryLoader::RelaxMeshGeometryBuffered(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
    assert(loadedMeshGeometries[meshGeometryID].usingBufferedCount > 0);
#endif
    loadedMeshGeometries[meshGeometryID].usingBufferedCount--;
    std::cout << "RelaxMeshGeometryBuffered ID = " << meshGeometryID << ", new count = " << loadedMeshGeometries[meshGeometryID].usingBufferedCount << "\n";
    if(loadedMeshGeometries[meshGeometryID].usingBufferedCount == 0) {
        UnBufferMeshGeometryData(meshGeometryID);
    }
}

MeshGeometryDataPtr MeshGeometryLoader::CopyMeshGeometryDataFromLoaded(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
#endif
    return std::make_shared<MeshGeometryData>(*(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr));
}

void MeshGeometryLoader::BufferMeshGeometryData(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    unsigned int size = loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getVertices()->size();
    assert(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getNormals()->size() == size);
    assert(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getTextureCoords()->size() == size);
#endif
    glGenBuffers(1, &(loadedMeshGeometries[meshGeometryID].meshVBO));
    
    glBindBuffer(GL_ARRAY_BUFFER, loadedMeshGeometries[meshGeometryID].meshVBO);
    unsigned int numVertices = loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getVertices()->size();
    unsigned int vertexStride = 3;
    unsigned int normalStride = 3;
    unsigned int textureCoordStride = 2;
    unsigned int stride = vertexStride + normalStride + textureCoordStride;
    unsigned int totalNumValues = numVertices * vertexStride + numVertices * normalStride + numVertices * textureCoordStride;
    std::unique_ptr<float[]> combinedVertexData = std::unique_ptr<float[]>(new float[totalNumValues]);
    for(unsigned int i = 0; i < numVertices; i++) {
        for(unsigned int j = 0; j < vertexStride; j++) {
            combinedVertexData.get()[i * stride + j] = (*(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getVertices()))[i][j];
        }
        for(unsigned int j = 0; j < normalStride; j++) {
            combinedVertexData.get()[i * stride + j + vertexStride] = (*(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getNormals()))[i][j];
        }
        for(unsigned int j = 0; j < textureCoordStride; j++) {
            combinedVertexData.get()[i * stride + j + vertexStride + normalStride] =
                    (*(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr->getTextureCoords()))[i][j];
        }
    }
    glBufferData(GL_ARRAY_BUFFER, totalNumValues * sizeof(float), combinedVertexData.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshGeometryLoader::UnBufferMeshGeometryData(const unsigned int meshGeometryID) {
    glDeleteBuffers(1, &(loadedMeshGeometries[meshGeometryID].meshVBO));
}

void MeshGeometryLoader::UnloadMeshGeometry(const unsigned int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID != 0 && meshGeometryID < spareID);
    assert(loadedMeshGeometries[meshGeometryID].usingCount == 0);
#endif
    for(std::unordered_map<unsigned int, MeshGeometryInfo>::iterator iter = loadedMeshGeometries.begin(); iter != loadedMeshGeometries.end(); iter++) {
        if(iter->first == meshGeometryID) {
            availableIDStack.push(iter->first);
            loadedMeshGeometries.erase(iter);
            break;
        }
    }
}

}
