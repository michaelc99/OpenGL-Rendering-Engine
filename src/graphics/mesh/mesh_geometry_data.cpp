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
        (*(this->vertices))[i] = (*(meshGeometryData.vertices))[i];
    }
    this->normals = std::make_shared<std::vector<Math::Vec3f>>();
    for(unsigned int i = 0; i < meshGeometryData.normals->size(); i++) {
        (*(this->normals))[i] = (*(meshGeometryData.normals))[i];
    }
    this->textureCoords = std::make_shared<std::vector<Math::Vec2f>>();
    for(unsigned int i = 0; i < meshGeometryData.textureCoords->size(); i++) {
        (*(this->textureCoords))[i] = (*(meshGeometryData.textureCoords))[i];
    }
}

/*
 * Class MeshGeometryLoader
 */
std::vector<MeshGeometryLoader::MeshGeometryInfo> MeshGeometryLoader::loadedMeshGeometries = std::vector<MeshGeometryLoader::MeshGeometryInfo>();

void MeshGeometryLoader::UnloadUnusedMeshGeometries() {
    for(unsigned int i = 0; i < loadedMeshGeometries.size(); i++) {
        if(loadedMeshGeometries[i].usingCount == 0) {
            UnloadMeshGeometry(i);
        }
    }
}

MeshGeometryDataPtr MeshGeometryLoader::GetMeshGeometryDataPtr(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
#endif
    return loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr;
}

void MeshGeometryLoader::BindMeshGeometry(const int meshGeometryID) {
    glBindBuffer(GL_ARRAY_BUFFER, loadedMeshGeometries[meshGeometryID].meshVBO);
}

int MeshGeometryLoader::LoadMeshFromMeshGeometryData(const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath) {
    // Check if the texture is already buffered
    if(modelFilePath != "") {
        for(unsigned int index = 0; index < loadedMeshGeometries.size(); index++) {
            if(loadedMeshGeometries[index].modelFilePath == modelFilePath) {
                return index;
            }
        }
    }
    
    MeshGeometryInfo meshGeometryInfo;
    meshGeometryInfo.modelFilePath = modelFilePath;
    meshGeometryInfo.meshGeometryDataPtr = meshGeometryDataPtr;
    meshGeometryInfo.meshVBO = 0;
    meshGeometryInfo.usingCount = 0;
    loadedMeshGeometries.push_back(meshGeometryInfo);
    
    int meshGeometryID = loadedMeshGeometries.size() - 1;
    return meshGeometryID;
}

void MeshGeometryLoader::UseLoadedMeshGeometry(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
#endif
    loadedMeshGeometries[meshGeometryID].usingCount++;
}

void MeshGeometryLoader::ReleaseLoadedMeshGeometry(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
    assert(loadedMeshGeometries[meshGeometryID].usingCount > 0);
#endif
    loadedMeshGeometries[meshGeometryID].usingCount--;
    if(loadedMeshGeometries[meshGeometryID].usingCount == 0) {
        UnBufferMeshGeometryData(meshGeometryID);
        if(loadedMeshGeometries[meshGeometryID].modelFilePath == "") {
            UnloadMeshGeometry(meshGeometryID);
        }
    }
}

void MeshGeometryLoader::RequireMeshGeometryBuffered(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
#endif
    if(loadedMeshGeometries[meshGeometryID].usingBufferedCount == 0) {
        BufferMeshGeometryData(meshGeometryID);
    }
    loadedMeshGeometries[meshGeometryID].usingBufferedCount++;
}

void MeshGeometryLoader::RelaxMeshGeometryBuffered(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
    assert(loadedMeshGeometries[meshGeometryID].usingBufferedCount > 0);
#endif
    loadedMeshGeometries[meshGeometryID].usingBufferedCount--;
    if(loadedMeshGeometries[meshGeometryID].usingBufferedCount == 0) {
        UnBufferMeshGeometryData(meshGeometryID);
    }
}

MeshGeometryDataPtr MeshGeometryLoader::CopyMeshGeometryDataFromLoaded(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
#endif
    return std::make_shared<MeshGeometryData>(*(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr));
}

void MeshGeometryLoader::BufferMeshGeometryData(const int meshGeometryID) {
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

void MeshGeometryLoader::UnBufferMeshGeometryData(const int meshGeometryID) {
    glDeleteBuffers(1, &(loadedMeshGeometries[meshGeometryID].meshVBO));
}

void MeshGeometryLoader::UnloadMeshGeometry(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometries.size());
    assert(loadedMeshGeometries[meshGeometryID].usingCount == 0);
#endif
    int index = 0;
    for(std::vector<MeshGeometryInfo>::iterator iter = loadedMeshGeometries.begin(); iter != loadedMeshGeometries.end(); iter++) {
        if(index == meshGeometryID) {
            loadedMeshGeometries.erase(iter);
            break;
        }
        index++;
    }
}

}
