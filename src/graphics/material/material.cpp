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

void Material::apply() const {
    // Do nothing
}

/*
 * Class TexturedMaterial
 */
TexturedMaterial::TexturedMaterial() : Material::Material() {}

TexturedMaterial::TexturedMaterial(const std::string& shaderProgramName, const std::vector<Texture> textures, const std::vector<float> textureMixingWeights)
    : Material::Material(shaderProgramName), textures(textures), textureMixingWeights(textureMixingWeights) {}

TexturedMaterial::TexturedMaterial(const ShaderProgramPtr shaderProgramPtr, const std::vector<Texture> textures, const std::vector<float> textureMixingWeights)
    : Material::Material(shaderProgramPtr), textures(textures), textureMixingWeights(textureMixingWeights) {}

void TexturedMaterial::apply() const {
#ifdef _DEBUG
    assert(getShaderProgramPtr().get() != nullptr);
    assert(textures.size() > 0);
    assert(textureMixingWeights.size() == textures.size());
#endif
    getShaderProgramPtr()->use();
//    getShaderProgramPtr()->setUniformInt("texture0", 0);
//    getShaderProgramPtr()->setUniformInt("texture1", 1);
//    for(unsigned int i = 0; i < textures.size(); i++) {
//        glActiveTexture(GL_TEXTURE0 + i);
//        textures[i].bind();
//    }
}

/*
 * Class UnTexturedMaterial
 */
UnTexturedMaterial::UnTexturedMaterial() : Material::Material() {}

UnTexturedMaterial::UnTexturedMaterial(const std::string& shaderProgramName, const std::vector<Math::Vec4f> colors, const std::vector<ColorType> colorTypes)
    : Material::Material(shaderProgramName), colors(colors), colorTypes(colorTypes) {}

UnTexturedMaterial::UnTexturedMaterial(const ShaderProgramPtr shaderProgramPtr, const std::vector<Math::Vec4f> colors, const std::vector<ColorType> colorTypes)
    : Material::Material(shaderProgramPtr), colors(colors), colorTypes(colorTypes) {}

void UnTexturedMaterial::apply() const {
    
}

}
