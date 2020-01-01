#ifndef MATERIAL_H
#define MATERIAL_H

#include <graphics/shaders/shaders.h>
#include <math/vector.h>
#include <graphics/texture/texture.h>
#include <vector>

namespace Engine {

class Material {
    public:
        Material(); // Get a shaderProgramPtr from ShaderLoader
        virtual ~Material();
        void virtual Use(); // ?????????
    protected:
        ShaderProgramPtr shaderProgramPtr;
};

class TexturedMaterial : Material {
    public:
        TexturedMaterial();
        ~TexturedMaterial();
        void Use();
    private:
        std::vector<Texture> textures;
        std::vector<float> textureMixingWeights;
};

class UnTexturedMaterial : Material {
    public:
        UnTexturedMaterial();
        ~UnTexturedMaterial();
        void Use();
    private:
        std::vector<Math::Vec3f> colors;
//        std::vector<float> colorMixingWeights; ????????
};

}

#endif //MATERIAL_H
