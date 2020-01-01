#include <iostream>
#include <thread>
#include <chrono>

#include <exceptions/render_exception.h>
#include <graphics/shaders/shaders.h>
#include <math/linear_math.h>
#include <fileio/image_reader.h>

#include <glad/glad.h> // Must include before GLFW
#include <GLFW/glfw3.h>

void myErrorCallback(int error, const char* description) {
    std::cout << "GLFW_ERROR: " << description << std::endl;
}

void myWindowCloseCallback(GLFWwindow* window) {
    std::cout << "Window close button pressed/requested." << std::endl;
}

int myFrameWidth = 0;
int myFrameHeight = 0;
void myFrameBufferResizeCallback(GLFWwindow* window, int frameWidth, int frameHeight) {
    std::cout << "New framebuffer size = (" << frameWidth << ", " << frameHeight << ")" << std::endl;
    glViewport(0, 0, frameWidth, frameHeight);
    myFrameWidth = frameWidth;
    myFrameHeight = frameHeight;
}

void myMousePosCallback(GLFWwindow* window, double x, double y) {
    std::cout << "Mouse pos = (" << x << ", " << y << ")" << std::endl;
}

float mixVal = 0;
float zVal = 1.0f;
void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    std::cout << key << " key was " << ((action == GLFW_PRESS) ? "pressed" : ((action == GLFW_RELEASE) ? "released" : "repeated")) << std::endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
        mixVal += 0.1;
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        mixVal -= 0.1;
    }
    
    if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        zVal *= 1.1f;
    }
    if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        zVal /= 1.1f;
    }
}

int main(void) {
    int errorNum = 0;
    
    glfwSetErrorCallback(myErrorCallback);
    
    if(!glfwInit()) {
        std::cout << "ERROR: GLFW initialization failed, aborting." << std::endl;
        errorNum = -1;
        return errorNum;
    }
    std::cout << "Success! GLFW initialized." << std::endl;
    
    try {
        // Set window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(900, 700, "My Window", NULL, NULL);
        if(!window) {
            throw Engine::RenderException(ERROR_INFO + ":ERROR: GLFW window creation failed, aborting.");
        }
        glfwMakeContextCurrent(window);
        
        glfwSetWindowCloseCallback(window, myWindowCloseCallback);
        glfwSetFramebufferSizeCallback(window, myFrameBufferResizeCallback);
        glfwSetCursorPosCallback(window, myMousePosCallback);
        glfwSetKeyCallback(window, myKeyCallback);
        
        // GLAD can load OpenGL functions after setting up context
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw Engine::RenderException(ERROR_INFO + ":ERROR: GLAD unable to load GL.");
        }
        std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
        
        glfwGetFramebufferSize(window, &myFrameWidth, &myFrameHeight);
        glViewport(0, 0, myFrameWidth, myFrameHeight);
        std::cout << "Frame buffer size = (" << myFrameWidth << ", " << myFrameHeight << ")" << std::endl;
        
        // Random info ////
        /*int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        std::cout << "Focused = " << focused << ", Window size = (" << windowWidth << ", " << windowHeight << ")" << std::endl;
        int majorVer = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
        int minorVer = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
        std::cout << "Context OpenGL version " << majorVer << "." << minorVer << std::endl;*/
        ////////////////////
        
        // Shader loading
        std::shared_ptr<Engine::ShaderProgram> shaderProgram;
        ADD_ERROR_INFO(
                shaderProgram = std::shared_ptr<Engine::ShaderProgram>(new Engine::ShaderProgram({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"basic_vertex_shader.vs.glsl", "basic_fragment_shader.fs.glsl"}))
        );
        
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        
        int imgWidth = 0;
        int imgHeight = 0;
        int imgNumChannels = 0;
        unsigned char* data;
        
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load("container.jpg", &imgWidth, &imgHeight, &imgNumChannels, 0);
        if(!data) {
            stbi_image_free(data);
            throw Engine::RenderException("bad image");
        }
        unsigned int texture0;
        glGenTextures(1, &texture0);
        // Texture0
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        
        data = stbi_load("awesomeface.png", &imgWidth, &imgHeight, &imgNumChannels, 0);
        if(!data) {
            stbi_image_free(data);
            throw Engine::RenderException("bad image");
        }
        unsigned int texture1;
        glGenTextures(1, &texture1);
        // Texture0
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        
        //////////////////////// Create VAOs /////
        // Vertices of triangle
        float vertices[] = {
            -0.5, -0.5, -9.0f,
            -0.5f, 0.5f, -10.0f,
            0.5f, 0.5f, -10.0f,
            0.5f, -0.5f, -9.0f
        };
        float colors[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };
        float textureCoords[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };
        float modelData[12 + 12 + 8];
        for(int i = 0; i < 12; i++) {
            modelData[i] = vertices[i];
        }
        for(int i = 0; i < 12; i++) {
            modelData[12 + i] = colors[i];
        }
        for(int i = 0; i < 8; i++) {
            modelData[12 + 12 + i] = textureCoords[i];
        }
        unsigned int indices[] = {
            0, 1, 2, 0, 2, 3
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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(4 * 3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // For texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2 * 4 * 3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        /////////////////////////////////////////
        
        // Set minimum of 1 frame time between swapping buffer
        glfwSwapInterval(1);
        while(!glfwWindowShouldClose(window)) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(17));
            glfwPollEvents();
            
            // Render to back buffer
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // DRAWING
            ADD_ERROR_INFO(shaderProgram->use());
            float time = ((int)(100.0f * glfwGetTime()) % 100) / 100.0f;
            
            ///////////////////////////
            ADD_ERROR_INFO(shaderProgram->setUniformFloat("mixVal", mixVal));
            ADD_ERROR_INFO(shaderProgram->setUniformInt("texture0", 0));
            ADD_ERROR_INFO(shaderProgram->setUniformInt("texture1", 1));
            
            Engine::Math::Mat4f myMatrix0(Engine::Math::createTranslationMat(
                    Engine::Math::createVec3<float>(0.0f, 0.0f, 9.5f)
            ));
            Engine::Math::Mat4f myMatrix1(Engine::Math::createRotationMat(
                    Engine::Math::createVec3<float>(0.0f, 1.0f, 1.0f), Engine::Math::toRadians(360.0f * time)
            ));
            Engine::Math::Mat4f myMatrix2(Engine::Math::createTranslationMat(
                    Engine::Math::createVec3<float>(0.0f, 0.0f, -9.5f)
            ));
            Engine::Math::Mat4f myMatrix3(Engine::Math::createTranslationMat(
                    Engine::Math::createVec3<float>(-0.5f, 0.0f, 0.0f)
            ));
            ADD_ERROR_INFO(shaderProgram->setUniformFloatMat("transform", myMatrix3 * myMatrix2 * myMatrix1 * myMatrix0));
            Engine::Math::Mat4f perspectiveMat = Engine::Math::createPerspectiveProjectionMat(Engine::Math::toRadians(45.0f), (float)myFrameWidth / (float)myFrameHeight, 1.0f, 100.0f);
            ADD_ERROR_INFO(shaderProgram->setUniformFloatMat("projectionMatrix", perspectiveMat));
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture0);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            
            glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            ////////////////////////////
            /*ADD_ERROR_INFO(shaderProgram->setUniformFloat("mixVal", mixVal));
            ADD_ERROR_INFO(shaderProgram->setUniformInt("texture0", 0));
            ADD_ERROR_INFO(shaderProgram->setUniformInt("texture1", 1));
            
            myMatrix0 = (Engine::Math::createTranslationMat(
                    Engine::Math::createVec3<float>(0.0f, 0.0f, 9.5f)
            ));
            myMatrix1 = (Engine::Math::createRotationMat(
                    Engine::Math::createVec3<float>(1.0f, 1.0f, 0.0f), Engine::Math::toRadians(-360.0f * time)
            ));
            myMatrix2 = (Engine::Math::createTranslationMat(
                    Engine::Math::createVec3<float>(0.0f, 0.0f, -9.5f)
            ));
            myMatrix3 = (Engine::Math::createTranslationMat(
                    Engine::Math::createVec3<float>(0.5f, 0.0f, 0.0f)
            ));
            ADD_ERROR_INFO(shaderProgram->setUniformFloatMat("transform", myMatrix3 * myMatrix2 * myMatrix1 * myMatrix0));
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture0);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            
            glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            */
            glfwSwapBuffers(window);
        }
        
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VAO);
        
        glfwDestroyWindow(window);
        
        // Terminate to free memory and resources
        glfwTerminate();
    }
    catch(Engine::GeneralException& e) {
        std::cerr << e.getMessage() << std::endl;
        glfwTerminate();
        errorNum = -1;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        errorNum = -1;
    }
    catch(...) {
        std::cerr << "ERROR: Unknown exception occurred." << std::endl;
        glfwTerminate();
        errorNum = -1;
    }
    
    return errorNum;
}
