#include <iostream>
#include <thread>
#include <chrono>

#include <exceptions/render_exception.h>
#include <shaders/shaders.h>
#include <math/linear_math.h>

#include <glad/glad.h> // Must include before GLFW
#include <GLFW/glfw3.h>

using namespace std;

void myErrorCallback(int error, const char* description) {
    cout << "GLFW_ERROR: " << description << endl;
}

void myWindowCloseCallback(GLFWwindow* window) {
    cout << "Window close button pressed/requested." << endl;
}

Vec2f mousePos;
int myFrameWidth, myFrameHeight;
void myFrameBufferResizeCallback(GLFWwindow* window, int frameWidth, int frameHeight) {
    cout << "New framebuffer size = (" << frameWidth << ", " << frameHeight << ")" << endl;
    glViewport(0, 0, frameWidth, frameHeight);
    myFrameWidth = frameWidth;
    myFrameHeight = frameHeight;
}

void myMousePosCallback(GLFWwindow* window, double x, double y) {
    cout << "Mouse pos = (" << x << ", " << y << ")" << endl;
    mousePos[0] = x;
    mousePos[1] = y;
}

void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    cout << key << " key was " << ((action == GLFW_PRESS) ? "pressed" : ((action == GLFW_RELEASE) ? "released" : "repeated")) << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    int errorNum = 0;
    
    glfwSetErrorCallback(myErrorCallback);
    
    if(!glfwInit()) {
        cout << "ERROR: GLFW initialization failed, aborting." << endl;
        errorNum = -1;
        return errorNum;
    }
    cout << "Success! GLFW initialized." << endl;
    
    try {
        // Set window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(500, 500, "My Window", NULL, NULL);
        if(!window) {
            throw RenderException(ERROR_INFO + ":ERROR: GLFW window creation failed, aborting.");
        }
        glfwMakeContextCurrent(window);
        
        glfwSetWindowCloseCallback(window, myWindowCloseCallback);
        glfwSetFramebufferSizeCallback(window, myFrameBufferResizeCallback);
        glfwSetCursorPosCallback(window, myMousePosCallback);
        glfwSetKeyCallback(window, myKeyCallback);
        
        // GLAD can load OpenGL functions after setting up context
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw RenderException(ERROR_INFO + ":ERROR: GLAD unable to load GL.");
        }
        cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << endl;
        
        glfwGetFramebufferSize(window, &myFrameWidth, &myFrameHeight);
        glViewport(0, 0, myFrameWidth, myFrameHeight);
        cout << "Frame buffer size = (" << myFrameWidth << ", " << myFrameHeight << ")" << endl;
        
        // Random info ////
        /*int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        cout << "Focused = " << focused << ", Window size = (" << windowWidth << ", " << windowHeight << ")" << endl;
        int majorVer = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
        int minorVer = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
        cout << "Context OpenGL version " << majorVer << "." << minorVer << endl;*/
        ////////////////////
        
        // Shader loading
        shared_ptr<ShaderProgram> shaderProgram;
        ADD_ERROR_INFO(
                shaderProgram = shared_ptr<ShaderProgram>(new ShaderProgram({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"basic_triangle.vs.glsl", "basic_triangle.fs.glsl"}))
        );
        
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        
        // Set minimum of 1 frame time between swapping buffer
        glfwSwapInterval(1);
        while(!glfwWindowShouldClose(window)) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(17));
            glfwPollEvents();
            
            // Render to back buffer
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            //////////////////////// Create VAOs /////
            float x = 2.0f * mousePos[0] / myFrameWidth - 1.0f;
            float y = -2.0f * mousePos[1] / myFrameHeight + 1.0f;
            
            // Vertices of triangle
            float vertices[] = {
                x, y, -0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
            };
            float colors[] = {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            };
            float modelData[9 * 2];
            for(int i = 0; i < 9; i++) {
                modelData[i] = vertices[i];
            }
            for(int i = 0; i < 9; i++) {
                modelData[i + 9] = colors[i];
            }
            unsigned int indices[] = {
                0, 1, 2
            };
            
            // FIRST VAO
            glBindVertexArray(VAO);
            // VBO Load vertices of shape into GPU memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(modelData), modelData, GL_STATIC_DRAW);
            // EBO Load indices of shape into GPU memory
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            // VertexAttribPointers Setup shader plumbing for vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // For colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * 3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            /////////////////////////////////////////
            
            // DRAWING
            ADD_ERROR_INFO(shaderProgram->use());
            float time = ((int)(100.0f * glfwGetTime()) % 200) / 200.0f -0.5f;
            Vec4f timeVec(time, time, time, 1.0f);
            ADD_ERROR_INFO(shaderProgram->setUniformVariable("colorFromApplication", timeVec));
            
            glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            
            glfwSwapBuffers(window);
        }
        
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VAO);
        
        glfwDestroyWindow(window);
        
        // Terminate to free memory and resources
        glfwTerminate();
    }
    catch(GeneralException& e) {
        cerr << e.getMessage() << endl;
        glfwTerminate();
        errorNum = -1;
    }
    catch(exception& e) {
        cerr << e.what() << endl;
        glfwTerminate();
        errorNum = -1;
    }
    catch(...) {
        cerr << "ERROR: Unknown exception occurred." << endl;
        glfwTerminate();
        errorNum = -1;
    }
    
    return errorNum;
}
