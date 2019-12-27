#include <graphics/shaders/shader_uniform.h>

namespace Engine {

GLint ShaderProgram::getUniformLocation(const std::string variableName) const {
    GLint uniformLocation = glGetUniformLocation(program, variableName.c_str());
    if(uniformLocation == -1) {
        throw RenderException("ERROR: Attempted to set invalid uniform variable \"" + variableName + "\".");
    }
    return uniformLocation;
}

void ShaderProgram::setUniformFloat(const std::string variableName, float val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1f(uniformLocation, val);
}

void ShaderProgram::setUniformDouble(const std::string variableName, double val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1d(uniformLocation, val);
}

void ShaderProgram::setUniformInt(const std::string variableName, int val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1i(uniformLocation, val);
}

void ShaderProgram::setUniformUInt(const std::string variableName, unsigned int val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1ui(uniformLocation, val);
}

};
