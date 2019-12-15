#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <exceptions/render_exception.h>

#include <glad/glad.h>

class ShaderObject {
    public:
        ShaderObject(GLenum type) : type(type), shader(0), filename("") {}
        ShaderObject(GLenum type, std::string filename);
        ~ShaderObject();
        void Load(std::string filename);
        void Compile();
        void Release();
        GLenum getType() { return type; }
        GLuint getShader() { return shader; }
        std::string getFileName() { return filename; }
    private:
        GLenum type;
        GLuint shader;
        std::string filename;
        std::string ReadShader(std::string filename);
};

class ShaderProgram {
    public:
        ShaderProgram(GLenum type);
        ShaderProgram(GLenum type, std::string filename);
        ~ShaderProgram();
        void AddShaderObject(ShaderObject& shaderObject);
        //ShaderObject RemoveShaderObject(GLuint shader);
        void Link();
        void Release();
        GLenum getType() { return type; }
        GLuint getProgram() { return program; }
    private:
        GLenum type;
        GLuint program;
        //std::vector<ShaderObject> shaderObjects;
        std::vector<std::string> shaderFileNames;
};

#endif //SHADER_LOADER_H
