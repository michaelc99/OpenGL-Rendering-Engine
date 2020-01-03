#include "mesh_geometry_data.h"

namespace Engine {

/*
 * Class MeshGeometryData
 */
MeshGeometryData::MeshGeometryData() {}

MeshGeometryData::MeshGeometryData(const VectorPtr<Math::Vec3f> vertices, const VectorPtr<Math::Vec3f> normals, const VectorPtr<Math::Vec2f> textureCoords) {
    this->vertices = vertices;
    this->normals = normals;
    this->textureCoords = textureCoords;
}

MeshGeometryData::MeshGeometryData(const MeshGeometryData& meshGeometryData) {
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
    if(loadedMeshGeometries[meshGeometryID].usingCount == 0) {
        BufferMeshGeometryData(meshGeometryID);
    }
    loadedMeshGeometries[meshGeometryID].usingCount++;
}

void MeshGeometryLoader::ReleaseLoadedMeshGeometry(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshes.size());
    assert(loadedMeshGeometry[meshGeometryID].usingCount > 0);
#endif
    loadedMeshGeometries[meshGeometryID].usingCount--;
    if(loadedMeshGeometries[meshGeometryID].usingCount == 0) {
        UnBufferMeshGeometryData(meshGeometryID);
        if(loadedMeshGeometries[meshGeometryID].modelFilePath == "") {
            UnloadMeshGeometry(meshGeometryID);
        }
    }
}

MeshGeometryDataPtr MeshGeometryLoader::CopyMeshGeometryDataFromLoaded(const int meshGeometryID) {
#ifdef _DEBUG
    assert(meshGeometryID > -1);
    assert(meshGeometryID < (int)loadedMeshGeometry.size());
#endif
    return std::make_shared<MeshGeometryData>(*(loadedMeshGeometries[meshGeometryID].meshGeometryDataPtr));
}

void MeshGeometryLoader::BufferMeshGeometryData(const int meshGeometryID) {
    
}

void MeshGeometryLoader::UnBufferMeshGeometryData(const int meshGeometryID) {
    
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
