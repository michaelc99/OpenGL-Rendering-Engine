/*
 * DO NOT INCLUDE THIS FILE, IT HAS TEMPLATES SPLIT UP
 * 
 * INCLUDE SHADERS.H
 */
#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H

#include <graphics/shaders/shader_loader.h>

namespace Engine {

template<size_t COLS>
void ShaderProgram::setUniformFloatVecs(const std::string variableName, const std::vector<Math::Vec<float, COLS>>& values) const {
#ifdef _DEBUG
    assert(COLS > 1 && COLS <= 4);
#endif
    const GLint uniformLocation = getUniformLocation(variableName);
    float data[COLS * values.size()];
    for(size_t i = 0; i < values.size(); i++) {
        for(size_t c = 0; c < COLS; c++) {
            data[i * COLS + c] = values[i][c];
        }
    }
    switch(COLS) {
        case 2:
            glUniform2fv(uniformLocation, values.size(), data);
            break;
        case 3:
            glUniform3fv(uniformLocation, values.size(), data);
            break;
        case 4:
            glUniform4fv(uniformLocation, values.size(), data);
            break;
    }
}

template<size_t COLS>
void ShaderProgram::setUniformDoubleVecs(const std::string variableName, const std::vector<Math::Vec<double, COLS>>& values) const {
#ifdef _DEBUG
    assert(COLS > 1 && COLS <= 4);
#endif
    const GLint uniformLocation = getUniformLocation(variableName);
    double data[COLS * values.size()];
    for(size_t i = 0; i < values.size(); i++) {
        for(size_t c = 0; c < COLS; c++) {
            data[i * COLS + c] = values[i][c];
        }
    }
    switch(COLS) {
        case 2:
            glUniform2dv(uniformLocation, values.size(), data);
            break;
        case 3:
            glUniform3dv(uniformLocation, values.size(), data);
            break;
        case 4:
            glUniform4dv(uniformLocation, values.size(), data);
            break;
    }
}

template<size_t COLS>
void ShaderProgram::setUniformIntVecs(const std::string variableName, const std::vector<Math::Vec<int, COLS>>& values) const {
#ifdef _DEBUG
    assert(COLS > 1 && COLS <= 4);
#endif
    const GLint uniformLocation = getUniformLocation(variableName);
    int data[COLS * values.size()];
    for(size_t i = 0; i < values.size(); i++) {
        for(size_t c = 0; c < COLS; c++) {
            data[i * COLS + c] = values[i][c];
        }
    }
    switch(COLS) {
        case 2:
            glUniform2iv(uniformLocation, values.size(), data);
            break;
        case 3:
            glUniform3iv(uniformLocation, values.size(), data);
            break;
        case 4:
            glUniform4iv(uniformLocation, values.size(), data);
            break;
    }
}

template<size_t COLS>
void ShaderProgram::setUniformUIntVecs(const std::string variableName, const std::vector<Math::Vec<unsigned int, COLS>>& values) const {
#ifdef _DEBUG
    assert(COLS > 1 && COLS <= 4);
#endif
    const GLint uniformLocation = getUniformLocation(variableName);
    unsigned int data[COLS * values.size()];
    for(size_t i = 0; i < values.size(); i++) {
        for(size_t c = 0; c < COLS; c++) {
            data[i * COLS + c] = values[i][c];
        }
    }
    switch(COLS) {
        case 2:
            glUniform2uiv(uniformLocation, values.size(), data);
            break;
        case 3:
            glUniform3uiv(uniformLocation, values.size(), data);
            break;
        case 4:
            glUniform4uiv(uniformLocation, values.size(), data);
            break;
    }
}

template<size_t COLS>
void ShaderProgram::setUniformFloatVec(const std::string variableName, const Math::Vec<float, COLS>& value) const {
    std::vector<Math::Vec<float, COLS>> values;
    values.push_back(value);
    setUniformFloatVecs(variableName, values);
}

template<size_t COLS>
void ShaderProgram::setUniformDoubleVec(const std::string variableName, const Math::Vec<double, COLS>& value) const {
    std::vector<Math::Vec<double, COLS>> values;
    values.push_back(value);
    setUniformDoubleVecs(variableName, values);
}

template<size_t COLS>
void ShaderProgram::setUniformIntVec(const std::string variableName, const Math::Vec<int, COLS>& value) const {
    std::vector<Math::Vec<int, COLS>> values;
    values.push_back(value);
    setUniformIntVecs(variableName, values);
}

template<size_t COLS>
void ShaderProgram::setUniformUIntVec(const std::string variableName, const Math::Vec<unsigned int, COLS>& value) const {
    std::vector<Math::Vec<unsigned int, COLS>> values;
    values.push_back(value);
    setUniformUIntVecs(variableName, values);
}

template<size_t ROWS, size_t COLS>
void ShaderProgram::setUniformFloatMats(const std::string variableName, const std::vector<Math::Mat<float, ROWS, COLS>>& values) const {
#ifdef _DEBUG
    assert(COLS > 1 && COLS <= 4);
    assert(ROWS > 1 && ROWS <= 4);
#endif
    const GLint uniformLocation = getUniformLocation(variableName);
    float data[ROWS * COLS * values.size()];
    for(size_t i = 0; i < values.size(); i++) {
        for(size_t r = 0; r < ROWS; r++) {
            for(size_t c = 0; c < COLS; c++) {
                data[i * (ROWS * COLS) + (r * ROWS + c)] = values[i][r][c];
            }
        }
    }
    switch(ROWS) {
        case 2:
            switch(COLS) {
                case 2:
                    glUniformMatrix2fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 3:
                    glUniformMatrix3x2fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 4:
                    glUniformMatrix4x2fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
            }
            break;
        case 3:
            switch(COLS) {
                case 2:
                    glUniformMatrix2x3fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 3:
                    glUniformMatrix3fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 4:
                    glUniformMatrix4x3fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
            }
            break;
        case 4:
            switch(COLS) {
                case 2:
                    glUniformMatrix2x4fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 3:
                    glUniformMatrix3x4fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 4:
                    glUniformMatrix4fv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
            }
            break;
    }
}

template<size_t ROWS, size_t COLS>
void ShaderProgram::setUniformDoubleMats(const std::string variableName, const std::vector<Math::Mat<double, ROWS, COLS>>& values) const {
#ifdef _DEBUG
    assert(COLS > 1 && COLS <= 4);
    assert(ROWS > 1 && ROWS <= 4);
#endif
    const GLint uniformLocation = getUniformLocation(variableName);
    double data[ROWS * COLS * values.size()];
    for(size_t i = 0; i < values.size(); i++) {
        for(size_t r = 0; r < ROWS; r++) {
            for(size_t c = 0; c < COLS; c++) {
                data[i * (ROWS * COLS) + (r * ROWS + c)] = values[i][r][c];
            }
        }
    }
    switch(ROWS) {
        case 2:
            switch(COLS) {
                case 2:
                    glUniformMatrix2dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 3:
                    glUniformMatrix3x2dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 4:
                    glUniformMatrix4x2dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
            }
            break;
        case 3:
            switch(COLS) {
                case 2:
                    glUniformMatrix2x3dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 3:
                    glUniformMatrix3dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 4:
                    glUniformMatrix4x3dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
            }
            break;
        case 4:
            switch(COLS) {
                case 2:
                    glUniformMatrix2x4dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 3:
                    glUniformMatrix3x4dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
                case 4:
                    glUniformMatrix4dv(uniformLocation, values.size(), GL_TRUE, data);
                    break;
            }
            break;
    }
}

template<size_t ROWS, size_t COLS>
void ShaderProgram::setUniformFloatMat(const std::string variableName, const Math::Mat<float, ROWS, COLS>& value) const {
    std::vector<Math::Mat<float, ROWS, COLS>> values;
    values.push_back(value);
    setUniformFloatMats(variableName, values);
}

template<size_t ROWS, size_t COLS>
void ShaderProgram::setUniformDoubleMat(const std::string variableName, const Math::Mat<double, ROWS, COLS>& value) const {
    std::vector<Math::Mat<double, ROWS, COLS>> values;
    values.push_back(value);
    setUniformDoubleMats(variableName, values);
}

};

#endif //SHADER_UNIFORM_H
