#include <graphics/shaders/shader_loader.h>

namespace Engine {

ShaderObject::ShaderObject(GLenum type, std::string filename) : ShaderObject(type) {
    load(filename);
}

ShaderObject::~ShaderObject() {
    release();
}

void ShaderObject::load(std::string filename) {
    this->filename = filename;
    std::string source;
    readFile(filename, source);
    const char* sourceCString = source.c_str();
    shader = glCreateShader(type);
    if(!glIsShader(shader)) {
        throw RenderException("ERROR: Failed to create shader object.");
    }
    glShaderSource(shader, 1, &sourceCString, NULL);
}

void ShaderObject::compile() {
    if(!shader) {
        throw RenderException("ERROR: Attempted to compile shader file \"" + filename + "\"that wasn't loaded.");
    }
    glCompileShader(shader);
    int compleStatus = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compleStatus);
    if(compleStatus != GL_TRUE) {
        char infoLog[1000] = {0};
        int infoLogLength = 0;
        glGetShaderInfoLog(shader, 1000, &infoLogLength, infoLog);
        throw RenderException("ERROR: Failed to compile shader file \"" + filename + "\". InfoLog:\n" + std::string(infoLog, infoLogLength));
    }
}

void ShaderObject::release() {
    if(shader != 0) {
        glDeleteShader(shader);
    }
    shader = 0;
    filename = "";
}

ShaderProgram::ShaderProgram(std::vector<GLenum>types, std::vector<std::string> filenames) : ShaderProgram() {
#ifdef _DEBUG
    assert(types.size() == filenames.size());
#endif
    create();
    std::vector<std::shared_ptr<ShaderObject>> shaderObjects;
    for(size_t i = 0; i < filenames.size(); i++) {
        std::shared_ptr<ShaderObject> shaderObject(new ShaderObject(types[i], filenames[i]));
        shaderObject->compile();
        shaderObjects.push_back(shaderObject);
    }
    for(size_t i = 0; i < filenames.size(); i++) {
        addShaderObject(shaderObjects[i]);
    }
    link();
}

ShaderProgram::~ShaderProgram() {
    release();
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& shaderProgram) {
    release();
    program = shaderProgram.program;
    linked = shaderProgram.linked;
    shaderFileNames = shaderProgram.shaderFileNames;
    return (*this);
}

void ShaderProgram::create() {
    program = glCreateProgram();
    if(!glIsProgram(program)) {
        throw RenderException("ERROR: Failed to create shader program.");
    }
}

/*
 * Assumes shaderObject has already been compiled successfully.
 */
void ShaderProgram::addShaderObject(std::shared_ptr<ShaderObject> shaderObject) {
    if(!program) {
        throw RenderException("ERROR: Attempted to add shader object to shader program before it was created.");
    }
    glAttachShader(program, shaderObject->getShader());
    shaderFileNames.push_back(shaderObject->getFileName());
    linked = false;
}

void ShaderProgram::detachShaderObject(std::shared_ptr<ShaderObject> shaderObject) {
    if(!program) {
        throw RenderException("ERROR: Attempted to add shader object to shader program before it was created.");
    }
    glDetachShader(program, shaderObject->getShader());
}

void ShaderProgram::link() {
    if(!program) {
        throw RenderException("ERROR: Attempted to link shader program that wasn't created.");
    }
    glLinkProgram(program);
    int linkStatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE) {
        char infoLog[1000] = {0};
        int infoLogLength = 0;
        glGetProgramInfoLog(program, 1000, &infoLogLength, infoLog);
        std::string errMessage = std::string("ERROR: Failed to link shader program with shader objects ");
        for(std::vector<std::string>::iterator shaderFileName = shaderFileNames.begin(); shaderFileName != shaderFileNames.end(); shaderFileName++) {
            errMessage = errMessage + "\"" + (*shaderFileName) + "\" ";
        }
        errMessage = errMessage + "\nInfoLog:\n" + std::string(infoLog, infoLogLength);
        throw RenderException(errMessage);
    }
    linked = true;
}

void ShaderProgram::release() {
    if(program != 0) {
        glDeleteProgram(program);
    }
    program = 0;
    linked = false;
    while(shaderFileNames.size() > 0) {
        shaderFileNames.pop_back();
    }
}

void ShaderProgram::use() const {
    if(!program) {
        throw RenderException("ERROR: Attempted to use shader program that wasn't created.");
    }
    if(!linked) {
        throw RenderException("ERROR: Attempted to use shader program that linked.");
    }
    glUseProgram(program);
}

};
