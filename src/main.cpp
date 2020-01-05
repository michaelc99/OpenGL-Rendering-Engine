#include <iostream>
#include <thread>
#include <chrono>

#include <exceptions/render_exception.h>
#include <fileio/image_reader.h>
#include <graphics/model/model_converter.h>

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
    //std::cout << "Mouse pos = (" << x << ", " << y << ")" << std::endl;
    if(myFrameWidth > 0 && myFrameHeight > 0)
        Engine::Mesh::myMousePos = Engine::Math::createVec2<float>((float)x/(float)myFrameWidth, (float)y/(float)myFrameHeight);
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
        
        // Setup
        Utility::ColladaModelConverter colladaModelConverter;
        ADD_ERROR_INFO(colladaModelConverter = Utility::ColladaModelConverter("wolf_test.dae"));
        Engine::Model model = Engine::Model(colladaModelConverter.getModelDataPtr());
        model.getModelDataPtr()->setMeshes({model.getModelDataPtr()->getMeshes()[0], model.getModelDataPtr()->getMeshes()[1]});
        
//        Engine::Model tempModel = Engine::Model(colladaModelConverter.getModelDataPtr());
//        Engine::ModelDataPtr tempModelDataPtr = std::make_shared<Engine::ModelData>(*(tempModel.getModelDataPtr().get()));
//        std::vector<Engine::Mesh> tempMeshes = {tempModelDataPtr->getMeshes()[0]};
//        tempModelDataPtr->setMeshes(tempMeshes);
//        Engine::Model model = Engine::Model(tempModelDataPtr);
        
        Engine::ShaderFiles files = {"myShader", "basic_vertex_shader.vs.glsl", "", "basic_fragment_shader.fs.glsl"};
        Engine::ShaderLoader::LoadShaderPrograms({files});
        std::vector<Engine::Mesh> meshes = model.getModelDataPtr()->getMeshes();
        Engine::TexturedMaterial texturedMaterial;
        texturedMaterial.setShaderProgramPtr(Engine::ShaderLoader::getShaderProgram("myShader"));
        texturedMaterial.setTextures({Engine::Texture("awesomeface.png", Engine::TextureType::TEXTURE_DIFFUSE)});
        texturedMaterial.setTextureMixingWeights({1.0f});
        meshes[0].setTexturedMaterial(texturedMaterial);
        meshes[1].setTexturedMaterial(texturedMaterial);
        model.getModelDataPtr()->setMeshes(meshes);
        
        // Set minimum of 1 frame time between swapping buffer
        glfwSwapInterval(1);
        while(!glfwWindowShouldClose(window)) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(17));
            glfwPollEvents();
            
            // Render to back buffer
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // DRAWING
            Engine::Mesh::myTime = ((int)(100.0f * glfwGetTime()) % 100) / 100.0f;
            model.render();
            
            glfwSwapBuffers(window);
        }
        
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
