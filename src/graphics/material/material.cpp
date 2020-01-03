#include "material.h"

namespace Engine {

/*
 * Class Material
 */
Material::Material() {}

Material::Material(const std::string& shaderProgramName) {
    shaderProgramPtr = ShaderLoader::getShaderProgram(shaderProgramName);
}

Material::Material(const ShaderProgramPtr shaderProgramPtr) : shaderProgramPtr(shaderProgramPtr) {}

void Material::apply() {
    
}

/*
 * Class TexturedMaterial
 */
TexturedMaterial::TexturedMaterial() : Material::Material() {}

TexturedMaterial::TexturedMaterial(const std::string& shaderProgramName, const std::vector<Texture> textures, const std::vector<float> textureMixingWeights)
    : Material::Material(shaderProgramName), textures(textures), textureMixingWeights(textureMixingWeights) {}

TexturedMaterial::TexturedMaterial(const ShaderProgramPtr shaderProgramPtr, const std::vector<Texture> textures, const std::vector<float> textureMixingWeights)
    : Material::Material(shaderProgramPtr), textures(textures), textureMixingWeights(textureMixingWeights) {}

void TexturedMaterial::apply() {
    
}

/*
 * Class UnTexturedMaterial
 */
UnTexturedMaterial::UnTexturedMaterial() : Material::Material() {}

UnTexturedMaterial::UnTexturedMaterial(const std::string& shaderProgramName, const std::vector<Math::Vec4f> colors, const std::vector<ColorType> colorTypes)
    : Material::Material(shaderProgramName), colors(colors), colorTypes(colorTypes) {}

UnTexturedMaterial::UnTexturedMaterial(const ShaderProgramPtr shaderProgramPtr, const std::vector<Math::Vec4f> colors, const std::vector<ColorType> colorTypes)
    : Material::Material(shaderProgramPtr), colors(colors), colorTypes(colorTypes) {}

void UnTexturedMaterial::apply() {
    
}

}
