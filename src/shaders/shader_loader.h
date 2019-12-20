#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <memory>
#include <exceptions/render_exception.h>
#include <math/vec.h>
#include <math/mat.h>

#include <glad/glad.h>

class ShaderObject {
    public:
        ShaderObject(GLenum type) : type(type), shader(0), filename("") {}
        ShaderObject(GLenum type, std::string filename);
        ~ShaderObject();
        void load(std::string filename);
        void compile();
        void release();
        GLenum getType() { return type; }
        GLuint getShader() { return shader; }
        std::string getFileName() { return filename; }
    private:
        std::string readShader(std::string filename);
        
        GLenum type;
        GLuint shader;
        std::string filename;
};

class ShaderProgram {
    public:
        ShaderProgram();
        ShaderProgram(std::vector<GLenum> types, std::vector<std::string> filenames);
        ~ShaderProgram();
        void addShaderObject(std::shared_ptr<ShaderObject> shaderObject);
        void link();
        void release();
        void use();
        
        void setUniformVariable(std::string variableName, float val);
        void setUniformVariable(std::string variableName, double val);
        void setUniformVariable(std::string variableName, int val);
        void setUniformVariable(std::string variableName, unsigned int val);
        void setUniformVariable(std::string variableName, bool val);
        template<typename T, size_t COLS>
        // NEED TO ASSERT COLS ARE IN ALLOWABLE GLSL RANGE AND T IS A VALID TYPE
        void setUniformVariable(std::string variableName, Vec<T, COLS> val);
        template<typename T, size_t ROWS, size_t COLS>
        // NEED TO ASSERT ROWS AND COLS ARE IN ALLOWABLE GLSL RANGE AND T IS A VALID TYPE
        void setUniformVariable(std::string variableName, Mat<T, ROWS, COLS> val);
        
        GLuint getProgram() { return program; }
    private:
        GLuint program;
        std::vector<std::string> shaderFileNames;
};

#endif //SHADER_LOADER_H
