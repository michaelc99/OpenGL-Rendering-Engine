#include <shaders/shader_uniform.h>

using namespace std;

GLint ShaderProgram::getUniformLocation(const string variableName) const {
    GLint uniformLocation = glGetUniformLocation(program, variableName.c_str());
    if(uniformLocation == -1) {
        throw RenderException("ERROR: Attempted to set invalid uniform variable \"" + variableName + "\".");
    }
    return uniformLocation;
}

void ShaderProgram::setUniformVariable(const string variableName, float val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1f(uniformLocation, val);
}

void ShaderProgram::setUniformVariable(const string variableName, double val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1d(uniformLocation, val);
}

void ShaderProgram::setUniformVariable(const string variableName, int val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1i(uniformLocation, val);
}

void ShaderProgram::setUniformVariable(const string variableName, unsigned int val) const {
    GLint uniformLocation = getUniformLocation(variableName);
    glUniform1ui(uniformLocation, val);
}
