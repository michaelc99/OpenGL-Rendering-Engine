#include <shaders/shader_loader.h>

using namespace std;

ShaderObject::ShaderObject(GLenum type, string filename) : ShaderObject(type) {
    Load(filename);
}

ShaderObject::~ShaderObject() {
    Release();
}

void ShaderObject::Load(string filename) {
    try {
        this->filename = filename;
        string source = ReadShader(filename);
        const char* sourceCString = source.c_str();
        shader = glCreateShader(type);
        if(!glIsShader(shader)) {
            throw RenderException("ERROR: Failed to create shader object.");
        }
        glShaderSource(shader, 1, &sourceCString, NULL);
    }
    catch(RenderException& e) {
        cerr << e.getMessage() << endl;
    }
}

void ShaderObject::Compile() {
    if(!shader) {
        throw RenderException("ERROR: Attempted to compile shader that wasn't loaded.");
    }
    glCompileShader(shader);
    int compleStatus = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compleStatus);
    if(compleStatus != GL_TRUE) {
        char infoLog[1000] = {0};
        int infoLogLength = 0;
        glGetShaderInfoLog(shader, 1000, &infoLogLength, infoLog);
        throw RenderException(string("ERROR: Failed to compile shader file \"") + filename + "\". InfoLog:\n" + string(infoLog, infoLogLength));
    }
}

void ShaderObject::Release() {
    if(shader != 0) {
        glDeleteShader(shader);
    }
    shader = 0;
    filename = "";
}

string ShaderObject::ReadShader(string filename) {
    ifstream inFile;
    string source = "";
    inFile = ifstream(filename, ios_base::in);
    if(inFile.fail()) {
        throw RenderException("ERROR: Failed to open shader source file: \"" + filename + "\"");
    }
    char line[1000];
    while(!inFile.eof()) {
        inFile.getline(line, 1000);
        if(!inFile.fail()) {
            source = source + string(line) + string("\n");
        }
    }
    inFile.close();
    return source;
}

ShaderProgram::ShaderProgram(GLenum type) : type(type), program(0) {
    program = glCreateProgram();
    if(!glIsProgram(program)) {
        throw RenderException("ERROR: Failed to create shader program.");
    }
}

ShaderProgram::ShaderProgram(GLenum type, std::string filename) : ShaderProgram(type) {
    ShaderObject shaderObject(type, filename);
    shaderObject.Compile();
    AddShaderObject(shaderObject);
}

ShaderProgram::~ShaderProgram() {
    Release();
}

/*
 * Assumes shaderObject has already been compiled successfully.
 */
void ShaderProgram::AddShaderObject(ShaderObject& shaderObject) {
    glAttachShader(program, shaderObject.getShader());
    shaderFileNames.push_back(shaderObject.getFileName());
}

void ShaderProgram::Link() {
    if(!program) {
        throw RenderException("ERROR: Attempted to link shader program that wasn't created.");
    }
    glLinkProgram(program);
    int linkStatus = 0;
    glGetShaderiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE) {
        char infoLog[1000] = {0};
        int infoLogLength = 0;
        glGetProgramInfoLog(program, 1000, &infoLogLength, infoLog);
        string errMessage = string("ERROR: Failed to link shader program with shader objects ");
        for(auto shaderFileName = shaderFileNames.begin(); shaderFileName != shaderFileNames.end(); shaderFileName++) {
            errMessage = errMessage + "\"" + (*shaderFileName) + "\" ";
        }
        errMessage = errMessage + "\nInfoLog:\n" + string(infoLog, infoLogLength);
        throw RenderException(errMessage);
    }
}

void ShaderProgram::Release() {
    if(program != 0) {
        glDeleteProgram(program);
    }
    program = 0;
    while(shaderFileNames.size() > 0) {
        shaderFileNames.pop_back();
    }
}
