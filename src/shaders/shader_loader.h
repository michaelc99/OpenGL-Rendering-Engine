#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <memory>
#include <exceptions/render_exception.h>

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
        /*void setUniform();
        void setUniform();
        void setUniform();
        void setUniform();
        void setUniform();*/
        
        GLuint getProgram() { return program; }
    private:
        GLuint program;
        std::vector<std::string> shaderFileNames;
};

#endif //SHADER_LOADER_H
