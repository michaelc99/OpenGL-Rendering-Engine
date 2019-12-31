#ifndef MESH_H
#define MESH_H

#include <math/vector.h>
#include <graphics/texture/texture.h>
#include <vector>
#include <memory>

namespace Engine {

class MeshData;
typedef std::shared_ptr<MeshData> MeshDataPtr;
class MeshLoader;

class Mesh {
    public:
        Mesh(const MeshDataPtr meshDataPtr, const std::vector<Texture>& textures);
        Mesh(const Mesh& mesh);
        ~Mesh();
    private:
        unsigned int meshVAO;
        std::vector<Texture> textures;
};

template<typename T>
using VectorPtr = std::shared_ptr<std::vector<T>>;

/*
 * MeshData contains the vertices, normals, texture coordinates, and indices for a mesh in system memory and the list of
 * texture objects for the mesh.
 */
class MeshData {
    public:
        MeshData();
        VectorPtr<Math::Vec3f> getVertices() { return vertices; }
        void setVertices(const VectorPtr<Math::Vec3f>& vertices) { this->vertices = vertices; }
        VectorPtr<Math::Vec3f> getNormals() { return normals; }
        void setNormals(const VectorPtr<Math::Vec3f>& normals) { this->normals = normals; }
        VectorPtr<Math::Vec2f> getTextureCoords() { return textureCoords; }
        void setTextureCoords(const VectorPtr<Math::Vec2f>& textureCoords) { this->textureCoords = textureCoords; }
        VectorPtr<Math::Vec3ui> getIndices() { return indices; }
        void setIndices(const VectorPtr<Math::Vec3ui>& indices) { this->indices = indices; }
        VectorPtr<Texture> getTextures() { return textures; }
        void setTextures(const VectorPtr<Texture>& textures) { this->textures = textures; }
    private:
        VectorPtr<Math::Vec3f> vertices;
        VectorPtr<Math::Vec3f> normals;
        VectorPtr<Math::Vec2f> textureCoords;
        VectorPtr<Math::Vec3ui> indices;
        VectorPtr<Texture> textures;
};

/*
 * MeshLoader handles buffering meshes into OpenGL from mesh data and maintaining a list of all unique buffered meshes by
 * OpenGL VAO name.
 */
class MeshLoader {
    friend Mesh;
    private:
        /*
         * Increments using count for mesh with OpenGL VAO name of meshVAO. Returns OpenGL VAO name for mesh;
         */
        static unsigned int useMeshFromLoaded(const unsigned int meshVAO);
        
        /*
         * Creates an OpenGL readable mesh from MeshData and buffers it using OpenGL. Returns OpenGL VAO name for mesh;
         */
        static unsigned int generateMeshFromData(const MeshDataPtr meshDataPtr);
        
        /*
         * Buffers mesh data from MeshDataPtr using OpenGL. Returns OpenGL VAO name for mesh;
         */
        static unsigned int bufferMeshData(const MeshDataPtr meshDataPtr);
        
        /*
         * Copies the buffered mesh data from meshVAO to MeshDataPtr.
         */
        static void copyMeshDataFromLoaded(const unsigned int meshVAO, const MeshDataPtr meshDataPtr);
        
        /*
         * Decrements using count for mesh with OpenGL VAO name of meshVAO. Returns OpenGL VAO name for mesh.
         */
        static void releaseMesh(const unsigned int meshVAO);
        
        /*
         * Deletes/unbuffers all buffered meshes with usage count less than 1.
         */
        static void unloadUnusedMeshes();
        
        struct MeshInfo {
            unsigned int numVertices;
            unsigned int numNormals;
            unsigned int numTextureCoords;
            unsigned int numIndices;
            unsigned int meshVAO;
            unsigned int count;
        };
        static std::vector<MeshInfo> loadedMeshes;
};

}

#endif //MESH_H
