#include "mesh.h"
#include <math/linear_math.h>

namespace Engine {

/*
 * Class Mesh
 */
Mesh::Mesh(const MeshDataPtr meshDataPtr, const TexturedMaterial texturedMaterial, const UnTexturedMaterial unTexturedMaterial, const std::string modelFilePath) {
    this->meshID = MeshLoader::LoadMeshFromMeshData(meshDataPtr, modelFilePath);
    MeshLoader::UseLoadedMesh(this->meshID);
    this->texturedMaterial = texturedMaterial;
    this->unTexturedMaterial = unTexturedMaterial;
}

Mesh::Mesh(const Mesh& mesh) {
    this->meshID = mesh.meshID;
    MeshLoader::UseLoadedMesh(this->meshID);
    this->texturedMaterial = mesh.texturedMaterial;
    this->unTexturedMaterial = mesh.unTexturedMaterial;
}

Mesh::~Mesh() {
    MeshLoader::ReleaseLoadedMesh(this->meshID);
}
float Mesh::myTime = 0.0f;
Math::Vec2f Mesh::myMousePos = Math::createVec2<float>(0.0f, 0.0f);
Math::Vec3f Mesh::myPos = Math::createVec3<float>(0.0f, 0.0f, 0.0f);
void Mesh::render() const {

    texturedMaterial.apply();
    MeshLoader::BindMesh(this->meshID);
    
    Math::Mat4f myMatrix0(Math::createTranslationMat(
            Math::createVec3<float>(-0.5f, -0.5f, -0.5f)
    ));
    Math::Mat4f myMatrix1(Math::createRotationMat(
            Math::createVec3<float>(1.0f, 1.0f, 1.0f), Math::toRadians(360.0f * myTime)
    ));
    Math::Mat4f myMatrix2(Math::createTranslationMat(
            Math::createVec3<float>(0.5f, 0.5f, 0.5f)
    ));
    Math::Mat4f myMatrix3(Math::createTranslationMat(
            Math::createVec3<float>(myPos[0], myPos[1], myPos[2])
    ));
    ADD_ERROR_INFO(texturedMaterial.getShaderProgramPtr()->setUniformFloatMat("transform", myMatrix3 * myMatrix2 * myMatrix1 * myMatrix0));
    Math::Mat4f perspectiveMat = Math::createPerspectiveProjectionMat(Math::toRadians(45.0f), (float)900 / (float)600, 1.0f, 100.0f);
    ADD_ERROR_INFO(texturedMaterial.getShaderProgramPtr()->setUniformFloatMat("projectionMatrix", perspectiveMat));
    
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
