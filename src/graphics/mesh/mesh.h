#ifndef MESH_H
#define MESH_H

#include <math/vector.h>
#include <graphics/model/material.h>
#include <vector>
#include <memory>

namespace Engine {

class Mesh {
    public:
        Mesh();
    private:
        std::vector<Math::Vec3f> vertices;
        std::vector<Math::Vec3f> normals;
        std::vector<Math::Vec2f> textureCoords;
        std::vector<Math::Vec3ui> indices;
        Material material;
};

class StaticMesh {
    public:
        StaticMesh();
    private:
        unsigned int VAO;
        Material material;
};

}

#endif //MESH_H
