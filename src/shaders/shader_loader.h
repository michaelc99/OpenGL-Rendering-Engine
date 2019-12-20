/*
 * DO NOT INCLUDE THIS FILE, IT HAS TEMPLATES SPLIT UP
 * 
 * INCLUDE SHADERS.H
 */
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
        ShaderProgram() : program(0), linked(false) {}
        ShaderProgram(std::vector<GLenum> types, std::vector<std::string> filenames);
        ~ShaderProgram();
        ShaderProgram& operator=(const ShaderProgram& shaderProgram);
        
        void create();
        void addShaderObject(std::shared_ptr<ShaderObject> shaderObject);
        void link();
        void detachShaderObject(std::shared_ptr<ShaderObject> shaderObject);
        void release();
        void use() const;
        
        GLint getUniformLocation(const std::string variableName) const;
        void setUniformVariable(const std::string variableName, float val) const;
        void setUniformVariable(const std::string variableName, double val) const;
        void setUniformVariable(const std::string variableName, int val) const;
        void setUniformVariable(const std::string variableName, unsigned int val) const;
        
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const std::vector<Vec<float, COLS>>& values) const;
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const std::vector<Vec<double, COLS>>& values) const;
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const std::vector<Vec<int, COLS>>& values) const;
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const std::vector<Vec<unsigned int, COLS>>& values) const;
        
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const Vec<float, COLS>& value) const;
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const Vec<double, COLS>& value) const;
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const Vec<int, COLS>& value) const;
        template<size_t COLS>
        void setUniformVariable(const std::string variableName, const Vec<unsigned int, COLS>& value) const;
        
        template<size_t ROWS, size_t COLS>
        void setUniformVariable(const std::string variableName, const std::vector<Mat<float, ROWS, COLS>>& values) const;
        template<size_t ROWS, size_t COLS>
        void setUniformVariable(const std::string variableName, const std::vector<Mat<double, ROWS, COLS>>& values) const;
        
        template<size_t ROWS, size_t COLS>
        void setUniformVariable(const std::string variableName, const Mat<float, ROWS, COLS>& value) const;
        template<size_t ROWS, size_t COLS>
        void setUniformVariable(const std::string variableName, const Mat<double, ROWS, COLS>& value) const;
        
        //void setUniformBlock(std::string blockName, std::shared_ptr<void> buf, size_t size) const;
        
        GLuint getProgram() { return program; }
    private:
        GLuint program;
        bool linked;
        std::vector<std::string> shaderFileNames;
};

#endif //SHADER_LOADER_H
