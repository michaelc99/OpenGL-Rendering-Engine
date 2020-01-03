#ifndef MESH_GEOMETRY_DATA_H
#define MESH_GEOMETRY_DATA_H

#include <math/vector.h>
#include <vector>
#include <memory>

namespace Engine {

template<typename T>
using VectorPtr = std::shared_ptr<std::vector<T>>;

/*
 * MeshData contains the vertices and normals for a group of meshes in system memory.
 */
class MeshGeometryData {
    public:
        MeshGeometryData(const VectorPtr<Math::Vec3f> vertices, const VectorPtr<Math::Vec3f> normals, const VectorPtr<Math::Vec2f> textureCoords);
        MeshGeometryData(const MeshGeometryData& meshGeometryData);
        
        VectorPtr<Math::Vec3f> getVertices() const { return vertices; }
        void setVertices(const VectorPtr<Math::Vec3f>& vertices) { this->vertices = vertices; }
        VectorPtr<Math::Vec3f> getNormals() const { return normals; }
        void setNormals(const VectorPtr<Math::Vec3f>& normals) { this->normals = normals; }
        VectorPtr<Math::Vec2f> getTextureCoords() const { return textureCoords; }
        void setTextureCoords(const VectorPtr<Math::Vec2f>& textureCoords) { this->textureCoords = textureCoords; }
        
    private:
        VectorPtr<Math::Vec3f> vertices;
        VectorPtr<Math::Vec3f> normals;
        VectorPtr<Math::Vec2f> textureCoords;
};
typedef std::shared_ptr<MeshGeometryData> MeshGeometryDataPtr;

/*
 * MeshGeometryLoader handles loading meshes from mesh data a list of loaded mesh geometries and buffering mesh geometries
 * into OpenGL.
 */
class MeshGeometryLoader {
    public:
        /*
         * Unloads all loaded mesh geometries with usage count less than 1 from system memory.
         */
        static void UnloadUnusedMeshGeometries();
        
        /*
         * Returns a MeshGeometryData pointer to mesh geometry data with index meshGeometryID from list of loaded mesh
         * geometries.
         */
        static MeshGeometryDataPtr GetMeshGeometryDataPtr(const int meshGeometryID);
        
        /*
         * Binds mesh geometry about of be rendered.
         */
        static void BindMeshGeometry(const int meshGeometryID);
        
        /*
         * Puts mesh geometry with data given by MeshGeometryDataPtr into list of loaded mesh geometries. Returns the index
         * of the mesh geometry from list of loaded mesh geometries. If modelFilePath is not empty and a MeshGeometryData is
         * found in the list of loaded mesh geometries then the index to the loaded instance will be returned and the
         * MeshGeometryDataPtr will not be added.
         */
        static int LoadMeshFromMeshGeometryData(const MeshGeometryDataPtr meshGeometryDataPtr, const std::string modelFilePath = "");
        
        /*
         * Increments using count for loaded mesh geometry with index meshGeometryID from list of loaded mesh geometries
         * and ensures it is buffered with OpenGL.
         */
        static void UseLoadedMeshGeometry(const int meshGeometryID);
        
        /*
         * Decrements using count for mesh geometry with index meshGeometryID from list of loaded mesh geometries.
         */
        static void ReleaseLoadedMeshGeometry(const int meshGeometryID);
        
        /*
         * Returns a deep copy of the loaded mesh geometry data with index meshGeometryID from list of loaded mesh
         * geometries.
         */
        static MeshGeometryDataPtr CopyMeshGeometryDataFromLoaded(const int meshGeometryID);
    private:
        /*
         * Buffers mesh geometry data to GPU from loaded mesh geometry list with index meshGeometryID.
         */
        static void BufferMeshGeometryData(const int meshGeometryID);
        
        /*
         * Deletes OpenGL buffer of mesh geometry with index meshGeometryID from loaded mesh geometry list.
         */
        static void UnBufferMeshGeometryData(const int meshGeometryID);
        
        /*
         * Unloads loaded mesh geometry from system memory. Removes the mesh geometry from list of loaded mesh
         * geometries.
         */
        static void UnloadMeshGeometry(const int meshGeometryID);
        
        struct MeshGeometryInfo {
            std::string modelFilePath;
            MeshGeometryDataPtr meshGeometryDataPtr;
            unsigned int meshVBO;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<MeshGeometryInfo> loadedMeshGeometries;
};

}

#endif //MESH_GEOMETRY_DATA_H
