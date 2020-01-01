#ifndef MODEL_H
#define MODEL_H

#include <graphics/mesh/mesh.h>
#include <graphics/texture/texture.h>
#include <string>
#include <vector>

namespace Engine {

class ModelData;
typedef std::shared_ptr<ModelData> ModelDataPtr;
class ModelLoader;

class Model {
    public:
        Model(const std::string filePath);
        Model(const ModelDataPtr modelDataPtr);
        Model(const Model& model);
        
        void addMesh(Mesh mesh);
    private:
        std::string filePath;
        std::vector<Mesh> meshes;
//        std::vector<Bone> bones; // Animation?????
};

class ModelData {
    public:
        ModelData();
        void addMeshData(MeshData mesh);
        
        void formatData();
    private:
        std::vector<MeshDataPtr> meshDataList;
        std::vector<TextureDataPtr> textureDataList;
//        std::vector<BoneDataPtr> boneDataList; // Animation?
};

/*
 * ModelLoader handles buffering all meshes for a model defined in a model file into OpenGL along with buffering all textures referenced
 * in the model file into OpenGL. Currently buffered meshes and textures will be used instead of reloading from file system.
 */
class ModelLoader {
    friend Model;
    public:
        
    private:
        /*
         * Loads texture from file system into system memory. Returns the index of texture from list of loaded textures.
         * If a texture with the same filename is already loaded, then the loaded instance will be used. If the texture is
         * not currently loaded then it will be loaded from the file system.
         */
        static int LoadTextureFromFile(const std::string filePath, unsigned int& width, unsigned int& height);
        
        /*
         * Puts texture with data given by TextureDataPtr into list of loaded textures. Returns the index of the texture
         * from list of loaded textures.
         */
        static int LoadTextureFromTextureData(const TextureDataPtr textureDataPtr);
        
        /*
         * Increments using count for loaded texture with index textureID from list of loaded textures and ensures it is
         * buffered with OpenGL.
         */
        static void UseLoadedTexture(const int textureID);
        
        /*
         * Decrements using count for texture with index textureID from list of loaded textures.
         */
        static void ReleaseLoadedTexture(const int textureID);
        
        /*
         * Copies the loaded texture data of texture with index textureID from list of loaded textures to TextureDataPtr.
         */
        static void CopyTextureDataFromLoaded(const int textureID, const TextureDataPtr textureDataPtr);
        
        /*
         * Buffers texture data to GPU from loaded texture list with index textureID.
         */
        static void BufferTextureData(const int textureID);
        
        /*
         * Deletes OpenGL texture buffer of texture from loaded texture list with index textureID.
         */
        static void UnbufferTextureData(const int textureID);
        
        /*
         * Unloads loaded texture from system memory. Removes the texture from list of loaded textures.
         */
        static void UnloadTexture(const int textureID);    
        /*
         * 
         * If a model with the same filename is already buffered, then the buffered copy will be used. Textures or meshes that are not currently
         * buffered will be loaded from the file system.
         */
        static unsigned int loadModelFromFile(std::string filePath);
        
        struct ModelInfo {
            std::string filePath;
            ModelDataPtr modelDataPtr;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<ModelInfo> loadedModels;
};

}

#endif //MODEL_H
