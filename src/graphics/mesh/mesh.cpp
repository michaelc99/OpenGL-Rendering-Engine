#include "mesh.h"
#include <math/linear_math.h>

namespace Engine {

/*
 * Class Mesh
 */
Mesh::Mesh(const MeshDataPtr meshDataPtr, const TexturedMaterial texturedMaterial, const UnTexturedMaterial unTexturedMaterial, const std::string modelFilePath)
    : texturedMaterial(texturedMaterial), unTexturedMaterial(unTexturedMaterial) {
    this->meshID = MeshLoader::LoadMeshFromMeshData(meshDataPtr);
    MeshLoader::UseLoadedMesh(this->meshID);
}

Mesh::Mesh(const Mesh& mesh) : texturedMaterial(mesh.texturedMaterial), unTexturedMaterial(mesh.unTexturedMaterial) {
    this->meshID = mesh.meshID;
    MeshLoader::UseLoadedMesh(this->meshID);
}

Mesh::~Mesh() {
    MeshLoader::ReleaseLoadedMesh(this->meshID);
}

void Mesh::render() const {
//        texturedMaterial.getShaderProgramPtr()->use();
//        unsigned int VAO;
//        glGenVertexArrays(1, &VAO);
//        
//        unsigned int VBO;
//        glGenBuffers(1, &VBO);
//        
//        unsigned int EBO;
//        glGenBuffers(1, &EBO);
//        float vertices[] = {
//            0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0
//        };
//        float colors[] = {
//            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//        };
//        float textureCoords[] = {
//            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//        };
//        float modelData[64];
//        for(int i = 0; i < 8; i++) {
//            modelData[i * 8] = vertices[3 * i];
//            modelData[i * 8 + 1] = vertices[3 * i + 1];
//            modelData[i * 8 + 2] = vertices[3 * i + 2];
//            modelData[i * 8 + 3] = colors[3 * i];
//            modelData[i * 8 + 4] = colors[3 * i + 1];
//            modelData[i * 8 + 5] = colors[3 * i + 2];
//            modelData[i * 8 + 6] = textureCoords[2 * i];
//            modelData[i * 8 + 7] = textureCoords[2 * i + 1];
//        }
//        unsigned int indices[] = {
//            0, 1, 2, 0, 2, 3, 3, 2, 5, 3, 5, 4, 1, 2, 5, 1, 5, 6, 0, 1, 7, 1, 7, 6, 7, 6, 4, 6, 4, 5, 0, 7, 3, 7, 3, 4
//        };
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(modelData), modelData, GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        // FIRST VAO
//        glBindVertexArray(VAO);
//        // VBO Load vertices of shape into GPU memory
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        // EBO Load indices of shape into GPU memory
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//        // VertexAttribPointers Setup shader plumbing for vertices
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        // For colors
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//        // For texture coords
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glBindVertexArray(0);
//        
//        glBindVertexArray(VAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
    texturedMaterial.apply();
    MeshLoader::BindMesh(this->meshID);
//    
//    /*Math::Mat4f myMatrix0(Math::createTranslationMat(
//            Math::createVec3<float>(0.0f, 0.0f, -9.5f)
//    ));
//    Math::Mat4f myMatrix1(Math::createRotationMat(
//            Math::createVec3<float>(0.0f, 1.0f, 1.0f), Math::toRadians(360.0f * 0.5f)
//    ));
//    Math::Mat4f myMatrix2(Math::createTranslationMat(
//            Math::createVec3<float>(0.0f, 0.0f, 9.5f)
//    ));*/
//    Math::Mat4f myMatrix3(Math::createTranslationMat(
//            Math::createVec3<float>(0.0f, 0.0f, -10.0f)
//    ));
//    ADD_ERROR_INFO(texturedMaterial.getShaderProgramPtr()->setUniformFloatMat("transform", myMatrix3/* * myMatrix2 * myMatrix1 * myMatrix0*/));
//    Math::Mat4f perspectiveMat = Math::createPerspectiveProjectionMat(Math::toRadians(45.0f), (float)900 / (float)600, 1.0f, 100.0f);
//    ADD_ERROR_INFO(texturedMaterial.getShaderProgramPtr()->setUniformFloatMat("projectionMatrix", perspectiveMat));
//    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, getMeshDataPtr()->getIndices()->size(), GL_UNSIGNED_INT, 0);
}

MeshDataPtr Mesh::getMeshDataPtr() const {
    return MeshLoader::GetMeshDataPtr(this->meshID);
}

MeshDataPtr Mesh::copyMeshData() const {
    return MeshLoader::CopyMeshDataFromLoaded(this->meshID);
}

}
