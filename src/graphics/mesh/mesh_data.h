#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <graphics/mesh/mesh_geometry_data.h>
#include <math/vector.h>
#include <vector>
#include <memory>
#include <cstring>
#include <stack>
#include <unordered_map>

#include <glad/glad.h>

namespace Engine {

template<typename T>
using VectorPtr = std::shared_ptr<std::vector<T>>;

/*
 * MeshData contains the texture coordinates and indices for a mesh in system memory.
 */
class MeshData {
    public:
        /*
         * Shallow copies indices and loads meshGeometryDataPtr using the mesh geometry loader.
         */
        MeshData(const VectorPtr<unsigned int> indices, const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath = "");
        
        /*
         * Deep copies mesh data into new mesh data.
         */
        MeshData(const MeshData& meshData);
        
        /*
         * Tells mesh geometry loader that this mesh is not longer using a geometry.
         */
        ~MeshData();
        
        /*
         * Returns a MeshGeometryDataPtr to a shallow copy of the mesh's geometry data in the list of (shared) loaded
         * mesh geometries.
         */
        MeshGeometryDataPtr getMeshGeometryDataPtr() const;
        
        /*
         * Adds the new geometry data to list of loaded geometry data and uses the new geometry data.
         */
        void setMeshGeometryDataPtr(const MeshGeometryDataPtr meshGeometryDataPtr);
        
        /*
         * Returns MeshGeometryDataPtr to a deep copy of this mesh's geometry data.
         */
        MeshGeometryDataPtr copyMeshGeometryData() const;
        
        unsigned int getMeshGeometryID() const { return meshGeometryID; }
        VectorPtr<unsigned int> getIndices() const { return indices; }
        void setIndices(const VectorPtr<unsigned int> indices) { this->indices = indices; }
    private:
        unsigned int meshGeometryID = 0;
        VectorPtr<unsigned int> indices;
};
typedef std::shared_ptr<MeshData> MeshDataPtr;

/*
 * MeshLoader handles loading meshes from mesh data a list of loaded meshes and buffering meshes into OpenGL.
 */
class MeshLoader {
    public:
        /*
         * Unloads all loaded meshes with usage count less than 1 from system memory.
         */
        static void UnloadUnusedMeshes();
        
        /*
         * Returns a MeshData pointer to mesh data with index meshID from list of loaded meshes.
         */
        static MeshDataPtr GetMeshDataPtr(const unsigned int meshID);
        
        /*
         * Binds mesh about of be rendered.
         */
        static void BindMesh(const unsigned int meshID);
        
        /*
         * Puts mesh with data given by MeshDataPtr into list of loaded meshes. Returns the index of the mesh from list
         * of loaded meshes.
         */
        static unsigned int LoadMeshFromMeshData(const MeshDataPtr meshDataPtr, const std::string filePath = "");
        
        /*
         * Increments using count for loaded mesh with index meshID from list of loaded meshes and ensures it is
         * buffered with OpenGL.
         */
        static void UseLoadedMesh(const unsigned int meshID);
        
        /*
         * Decrements using count for mesh with index meshID from list of loaded meshes.
         */
        static void ReleaseLoadedMesh(const unsigned int meshID);
        
        /*
         * Returns a deep copy of the loaded mesh data with index meshID from list of loaded meshes.
         */
        static MeshDataPtr CopyMeshDataFromLoaded(const unsigned int meshID);
    private:
        /*
         * Buffers mesh data to GPU from loaded mesh list with index meshID.
         */
        static void BufferMeshData(const unsigned int meshID);
        
        /*
         * Deletes OpenGL buffer of mesh with index meshID from loaded mesh list.
         */
        static void UnBufferMeshData(const unsigned int meshID);
        
        /*
         * Unloads loaded mesh from system memory. Removes the mesh from list of loaded meshes.
         */
        static void UnloadMesh(const unsigned int meshID);
        
        struct MeshInfo {
            std::string modelFilePath;
            MeshDataPtr meshDataPtr;
            unsigned int meshEBO;
            unsigned int meshVAO;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static unsigned int spareID;
        static std::stack<unsigned int> availableIDStack;
        static std::unordered_map<unsigned int, MeshInfo> loadedMeshes;
};

}

#endif //MESH_DATA_H
