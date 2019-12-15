#include <iostream>
#include <thread>
#include <chrono>

#include <exceptions/render_exception.h>
#include <shaders/shader_loader.h>

#include <glad/glad.h> // Must include before GLFW
#include <GLFW/glfw3.h>

using namespace std;

void myErrorCallback(int error, const char* description) {
    cout << "GLFW_ERROR: " << description << endl;
}

void myWindowCloseCallback(GLFWwindow* window) {
    cout << "Window close button pressed/requested." << endl;
}

void myFrameBufferResizeCallback(GLFWwindow* window, int frameWidth, int frameHeight) {
    cout << "New framebuffer size = (" << frameWidth << ", " << frameHeight << ")" << endl;
    glViewport(0, 0, frameWidth, frameHeight);
}

void myMousePosCallback(GLFWwindow* window, double x, double y) {
    //cout << "Mouse pos = (" << x << ", " << y << ")" << endl;
}

void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    cout << key << " key was " << ((action == GLFW_PRESS) ? "pressed" : ((action == GLFW_RELEASE) ? "released" : "repeated")) << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    
    glfwSetErrorCallback(myErrorCallback);
    
    if(!glfwInit()) {
        cout << "ERROR: GLFW initialization failed, aborting." << endl;
        return -1;
    }
    cout << "Success! GLFW initialized." << endl;
    
    try {
        // Set window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(500, 500, "My Window", NULL, NULL);
        if(!window) {
            throw RenderException("ERROR: GLFW window creation failed, aborting.");
        }
        glfwMakeContextCurrent(window);
        
        glfwSetWindowCloseCallback(window, myWindowCloseCallback);
        glfwSetFramebufferSizeCallback(window, myFrameBufferResizeCallback);
        glfwSetCursorPosCallback(window, myMousePosCallback);
        glfwSetKeyCallback(window, myKeyCallback);
        
        // GLAD can load OpenGL functions after setting up context
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw RenderException("ERROR: GLAD unable to load GL.");
        }
        cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << endl;
        
        int frameWidth, frameHeight;
        glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
        glViewport(0, 0, frameWidth, frameHeight);
        cout << "Frame buffer size = (" << frameWidth << ", " << frameHeight << ")" << endl;
        
        // Random info ////
        int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        cout << "Focused = " << focused << ", Window size = (" << windowWidth << ", " << windowHeight << ")" << endl;
        int majorVer = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
        int minorVer = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
        cout << "Context OpenGL version " << majorVer << "." << minorVer << endl;
        ////////////////////
        
        // Shader loading
        ShaderObject vertexShader = ShaderObject(GL_VERTEX_SHADER, "basic_triangle.vs.glsl");
        vertexShader.Compile();
        ShaderObject fragShader = ShaderObject(GL_FRAGMENT_SHADER, "basic_triangle.fs.glsl");
        fragShader.Compile();
        ShaderProgram shaderProgram = ShaderProgram(GL_VERTEX_SHADER);
        shaderProgram.AddShaderObject(vertexShader);
        shaderProgram.AddShaderObject(fragShader);
        shaderProgram.Link();
        vertexShader.Release();
        fragShader.Release();
        
        // Set minimum of 1 frame time between swapping buffer
        glfwSwapInterval(1);
        while(!glfwWindowShouldClose(window)) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(17));
            glfwPollEvents();
            
            // Render to back buffer
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // Vertices of triangle
            float vertices[][9] = {
                {
                    0.0f, 0.0f, 0.0f,
                    1.0f, 0.5f, 0.0f,
                    1.0f, -0.5f, 0.0f
                },
                {
                    0.0f, 0.0f, 0.0f,
                    -1.0f, 0.5f, 0.0f,
                    -1.0f, -0.5f, 0.0f
                }
            };
            
            // ID for VBO
            unsigned int VBO[2];
            glGenBuffers(2, VBO);
            
            // Load first triangle into GPU memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0], GL_STATIC_DRAW);
            
            // Load second triangle into GPU memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[1]), vertices[1], GL_STATIC_DRAW);
            
            // Use shader program
            glUseProgram(shaderProgram.getProgram());
            
            // ????????????????
            
            glDeleteBuffers(2, VBO);
            
            glfwSwapBuffers(window);
        }
        
        glfwDestroyWindow(window);
        
        // Terminate to free memory and resources
        glfwTerminate();
    }
    catch(RenderException& e) {
        cerr << e.getMessage() << endl;
        glfwTerminate();
        return -1;
    }
    catch(exception& e) {
        cerr << e.what() << endl;
        glfwTerminate();
        return -1;
    }
    
    return 0;
}
