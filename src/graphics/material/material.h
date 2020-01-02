#ifndef MATERIAL_H
#define MATERIAL_H

#include <graphics/shaders/shaders.h>
#include <math/vector.h>
#include <graphics/texture/texture.h>
#include <vector>

namespace Engine {

class Material {
        Material();
        
        void apply();
        ShaderProgramPtr getShaderProgramPtr() const { return shaderProgramPtr; }
        void setShaderProgramPtr(const ShaderProgramPtr shaderProgramPtr) { this->shaderProgramPtr = shaderProgramPtr; }
    protected:
        ShaderProgramPtr shaderProgramPtr;
};

class TexturedMaterial : Material {
    public:
        TexturedMaterial();
        
        void apply();
    private:
        std::vector<Texture> textures;
        std::vector<float> textureMixingWeights;
};

class UnTexturedMaterial : Material {
    public:
        UnTexturedMaterial();
        
        void apply();
    private:
        std::vector<Math::Vec4f> colors;
//        std::vector<float> colorMixingWeights; ????????
};

}

#endif //MATERIAL_H
