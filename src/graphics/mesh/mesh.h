#ifndef MESH_H
#define MESH_H

#include <math/vector.h>
#include <graphics/material/material.h>
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
        Material material;
        int meshID = -1;
};

template<typename T>
using VectorPtr = std::shared_ptr<std::vector<T>>;

/*
 * MeshData contains the vertices, normals, texture coordinates, and indices for a mesh in system memory and the list of
 * texture objects for the mesh. ModelLoader handles loading the textures from TextureLoader.
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
    public:
        
    private:
        
        
        struct MeshInfo {
            MeshDataPtr meshDataPtr;
            unsigned int meshVAO;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<MeshInfo> loadedMeshes;
};

}

#endif //MESH_H
