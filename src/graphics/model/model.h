#ifndef MODEL_H
#define MODEL_H

#include <graphics/mesh/mesh.h>
#include <vector>

namespace Engine {

/*class StaticModel {
    public:
        StaticModel();
        void addMesh(StaticMesh staticMesh);
    private:
        std::vector<StaticMesh> meshes;
};*/

class Model {
    public:
        Model();
        void addMesh(Mesh mesh);
    private:
        std::vector<Mesh> meshes;
//        std::vector<Bone> bones; // Animation?
};

}

#endif //MODEL_H
