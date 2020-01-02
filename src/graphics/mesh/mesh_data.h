#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <math/vector.h>
#include <vector>
#include <memory>

namespace Engine {

template<typename T>
using VectorPtr = std::shared_ptr<std::vector<T>>;

/*
 * MeshData contains the vertices, normals, texture coordinates, and indices, and material for a mesh in system memory.
 */
class MeshData {
    public:
        MeshData();
        MeshData(const VectorPtr<Math::Vec3f> vertices, const VectorPtr<Math::Vec3f> normals, const VectorPtr<Math::Vec2f> textureCoords,
                const VectorPtr<Math::Vec3ui> indices);
        MeshData(const MeshData& meshData);
        
        VectorPtr<Math::Vec3f> getVertices() const { return vertices; }
        void setVertices(const VectorPtr<Math::Vec3f>& vertices) { this->vertices = vertices; }
        VectorPtr<Math::Vec3f> getNormals() const { return normals; }
        void setNormals(const VectorPtr<Math::Vec3f>& normals) { this->normals = normals; }
        VectorPtr<Math::Vec2f> getTextureCoords() const { return textureCoords; }
        void setTextureCoords(const VectorPtr<Math::Vec2f>& textureCoords) { this->textureCoords = textureCoords; }
        VectorPtr<Math::Vec3ui> getIndices() const { return indices; }
        void setIndices(const VectorPtr<Math::Vec3ui>& indices) { this->indices = indices; }
    private:
        VectorPtr<Math::Vec3f> vertices;
        VectorPtr<Math::Vec3f> normals;
        VectorPtr<Math::Vec2f> textureCoords;
        VectorPtr<Math::Vec3ui> indices;
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
        static MeshDataPtr GetMeshDataPtr(const int meshID);
        
        /*
         * Binds mesh about of be rendered.
         */
        static void BindMesh(const int meshID);
        
        /*
         * Puts mesh with data given by MeshDataPtr into list of loaded meshes. Returns the index of the mesh from list
         * of loaded meshes.
         */
        static int LoadMeshFromMeshData(const MeshDataPtr meshDataPtr);
        
        /*
         * Increments using count for loaded mesh with index meshID from list of loaded meshes and ensures it is
         * buffered with OpenGL.
         */
        static void UseLoadedMesh(const int meshID);
        
        /*
         * Decrements using count for mesh with index meshID from list of loaded meshes.
         */
        static void ReleaseLoadedMesh(const int meshID);
        
        /*
         * Returns a deep copy of the loaded mesh data with index meshID from list of loaded meshes.
         */
        static MeshDataPtr CopyMeshDataFromLoaded(const int meshID);
    private:
        /*
         * Buffers mesh data to GPU from loaded mesh list with index meshID.
         */
        static void BufferMeshData(const int meshID);
        
        /*
         * Deletes OpenGL buffer of mesh with index meshID from loaded mesh list.
         */
        static void UnBufferMeshData(const int meshID);
        
        /*
         * Unloads loaded mesh from system memory. Removes the mesh from list of loaded meshes.
         */
        static void UnloadMesh(const int meshID);
        
        struct MeshInfo {
            MeshDataPtr meshDataPtr;
            unsigned int meshVAO;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<MeshInfo> loadedMeshes;
};

}

#endif //MESH_DATA_H
