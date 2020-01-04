#ifndef MATERIAL_H
#define MATERIAL_H

#include <graphics/shaders/shaders.h>
#include <math/vector.h>
#include <graphics/texture/texture.h>
#include <vector>
#include <cassert>

namespace Engine {

class Material {
    public:
        Material();
        Material(const std::string& shaderProgramName);
        Material(const ShaderProgramPtr shaderProgramPtr);
        
        void apply() const;
        
        ShaderProgramPtr getShaderProgramPtr() const { return shaderProgramPtr; }
        void setShaderProgramPtr(const ShaderProgramPtr shaderProgramPtr) { this->shaderProgramPtr = shaderProgramPtr; }
    private:
        ShaderProgramPtr shaderProgramPtr;
};

class TexturedMaterial : public Material {
    public:
        TexturedMaterial();
        TexturedMaterial(const std::string& shaderProgramName, const std::vector<Texture> textures, const std::vector<float> textureMixingWeights);
        TexturedMaterial(const ShaderProgramPtr shaderProgramPtr, const std::vector<Texture> textures, const std::vector<float> textureMixingWeights);
        
        void apply() const;
        
        std::vector<Texture> getTextures() const { return textures; }
        void setTextures(const std::vector<Texture> textures) { this->textures = textures; }
        std::vector<float> getTextureMixingWeights() const { return textureMixingWeights; }
        void setTextureMixingWeights(const std::vector<float> textureMixingWeights) { this->textureMixingWeights = textureMixingWeights; }
    private:
        std::vector<Texture> textures;
        std::vector<float> textureMixingWeights;
};

enum ColorType {
    COLOR_DIFFUSE,
    COLOR_SPECULAR
};

class UnTexturedMaterial : public Material {
    public:
        UnTexturedMaterial();
        UnTexturedMaterial(const std::string& shaderProgramName, const std::vector<Math::Vec4f> colors, const std::vector<ColorType> colorTypes);
        UnTexturedMaterial(const ShaderProgramPtr shaderProgramPtr, const std::vector<Math::Vec4f> colors, const std::vector<ColorType> colorTypes);
        
        void apply() const;
        
        std::vector<Math::Vec4f> getColors() const { return colors; }
        void setColors(const std::vector<Math::Vec4f> colors) { this->colors = colors; }
        std::vector<ColorType> getColorTypes() const { return colorTypes; }
        void setColorTypes(const std::vector<ColorType> colorTypes) { this->colorTypes = colorTypes; }
    private:
        std::vector<Math::Vec4f> colors;
        std::vector<ColorType> colorTypes;
};

}

#endif //MATERIAL_H
