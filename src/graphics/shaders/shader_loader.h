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
#include <math/vector.h>
#include <math/matrix.h>
#include <fileio/fileio.h>

#include <glad/glad.h>

namespace Engine {

class ShaderObject {
    public:
        ShaderObject(const GLenum type) : type(type), shader(0), filePath("") {}
        ShaderObject(const GLenum type, const std::string filePath);
        ~ShaderObject();
        void load(const std::string filePath);
        void compile();
        void release();
        GLenum getType() { return type; }
        GLuint getShader() { return shader; }
        std::string getFileName() { return filePath; }
    private:
        GLenum type;
        GLuint shader;
        std::string filePath;
};

class ShaderProgram {
    public:
        ShaderProgram() : program(0), linked(false), shaderProgramName("") {}
        ShaderProgram(const std::vector<GLenum> types, const std::vector<std::string> filePaths, const std::string shaderProgramName);
        ~ShaderProgram();
        ShaderProgram& operator=(const ShaderProgram& shaderProgram);
        
        void create();
        void addShaderObject(const std::shared_ptr<ShaderObject> shaderObject);
        void link();
        void detachShaderObject(const std::shared_ptr<ShaderObject> shaderObject);
        void release();
        void use() const;
        
        GLint getUniformLocation(const std::string variableName) const;
        void setUniformFloat(const std::string variableName, float val) const;
        void setUniformDouble(const std::string variableName, double val) const;
        void setUniformInt(const std::string variableName, int val) const;
        void setUniformUInt(const std::string variableName, unsigned int val) const;
        
        template<size_t COLS>
        void setUniformFloatVecs(const std::string variableName, const std::vector<Math::Vec<float, COLS>>& values) const;
        template<size_t COLS>
        void setUniformDoubleVecs(const std::string variableName, const std::vector<Math::Vec<double, COLS>>& values) const;
        template<size_t COLS>
        void setUniformIntVecs(const std::string variableName, const std::vector<Math::Vec<int, COLS>>& values) const;
        template<size_t COLS>
        void setUniformUIntVecs(const std::string variableName, const std::vector<Math::Vec<unsigned int, COLS>>& values) const;
        
        template<size_t COLS>
        void setUniformFloatVec(const std::string variableName, const Math::Vec<float, COLS>& value) const;
        template<size_t COLS>
        void setUniformDoubleVec(const std::string variableName, const Math::Vec<double, COLS>& value) const;
        template<size_t COLS>
        void setUniformIntVec(const std::string variableName, const Math::Vec<int, COLS>& value) const;
        template<size_t COLS>
        void setUniformUIntVec(const std::string variableName, const Math::Vec<unsigned int, COLS>& value) const;
        
        template<size_t ROWS, size_t COLS>
        void setUniformFloatMats(const std::string variableName, const std::vector<Math::Mat<float, ROWS, COLS>>& values) const;
        template<size_t ROWS, size_t COLS>
        void setUniformDoubleMats(const std::string variableName, const std::vector<Math::Mat<double, ROWS, COLS>>& values) const;
        
        template<size_t ROWS, size_t COLS>
        void setUniformFloatMat(const std::string variableName, const Math::Mat<float, ROWS, COLS>& value) const;
        template<size_t ROWS, size_t COLS>
        void setUniformDoubleMat(const std::string variableName, const Math::Mat<double, ROWS, COLS>& value) const;
        
        //void setUniformBlock(std::string blockName, std::shared_ptr<void> buf, size_t size) const;
        
        GLuint getProgram() { return program; }
        std::string getShaderProgramName() { return shaderProgramName; }
    private:
        GLuint program;
        bool linked;
        std::string shaderProgramName;
        std::vector<std::string> shaderFileNames;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

struct ShaderFiles {
    std::string shaderProgramName = "";
    std::string vertexShaderFilePath = "";
    std::string geometryShaderFilePath = "";
    std::string fragmentShaderFilePath = "";
};

class ShaderLoader {
    public:
        /*
         * Loads, compiles, and buffers a list of shader programs using OpenGL.
         */
        static void LoadShaderPrograms(const std::vector<ShaderFiles>& shaderFiles);
        
        /*
         * Returns pointer to ShaderProgram buffered with OpenGL from list of buffered shader programs with name
         * shaderProgramName.
         */
        static ShaderProgramPtr getShaderProgram(const std::string& shaderProgramName);
    private:
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<ShaderProgramPtr> loadedShaderPrograms;
};

};

#endif //SHADER_LOADER_H
