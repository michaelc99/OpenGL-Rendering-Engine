#ifndef MESH_H
#define MESH_H

#include <graphics/mesh/mesh_data.h>
#include <graphics/material/material.h>
#include <vector>

namespace Engine {

class Mesh {
    public:
        Mesh(const MeshDataPtr meshDataPtr, const TexturedMaterial texturedMaterial, const UnTexturedMaterial unTexturedMaterial);
        Mesh(const Mesh& mesh);
        ~Mesh();
        
        void render() const;
        
        /*
         * Returns a MeshDataPtr to a shallow copy of the mesh's data in the list of (shared) loaded meshes.
         */
        MeshDataPtr getMeshDataPtr() const;
        
        /*
         * Returns MeshDataPtr to a deep copy of this mesh's data.
         */
        MeshDataPtr copyMeshData() const;
        
        TexturedMaterial getTexturedMaterial() const { return texturedMaterial; }
        void setTexturedMaterial(const TexturedMaterial texturedMaterial) { this->texturedMaterial = texturedMaterial; }
        UnTexturedMaterial getUnTexturedMaterial() const { return unTexturedMaterial; }
        void setUnTexturedMaterial(const UnTexturedMaterial unTexturedMaterial) { this->unTexturedMaterial = unTexturedMaterial; }
    private:
        int meshID = -1;
        TexturedMaterial texturedMaterial;
        UnTexturedMaterial unTexturedMaterial;
};

}

#endif //MESH_H
