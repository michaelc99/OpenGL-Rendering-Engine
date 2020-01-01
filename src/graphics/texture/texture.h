#ifndef TEXTURE_H
#define TEXTURE_H

#include <fileio/image_reader.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>

#include <glad/glad.h>

namespace Engine {

enum TextureType {
    DIFFUSE,
    SPECULAR
};

class TextureData;
typedef std::shared_ptr<TextureData> TextureDataPtr;
class TextureLoader;

class Texture {
    public:
        Texture(const std::string filePath, const TextureType type);
        Texture(const TextureDataPtr textureDataPtr, const TextureType type);
        Texture(const Texture& texture);
        ~Texture();
        
        /*
         * Returns TextureDataPtr to a copy of this texture's data.
         */
        TextureDataPtr copyTextureData();
        
        std::string getFilePath() const { return filePath; }
        TextureType getType() const { return type; }
        void setType(TextureType type) { this->type = type; }
        unsigned int getWidth() const { return width; }
        unsigned int getHeight() const { return height; }
    private:
        std::string filePath;
        TextureType type;
        int textureID = -1;
        unsigned int width;
        unsigned int height;
};

/*
 * TextureData contains the pixel data for a texture in system memory.
 */
class TextureData {
    public:
        TextureData();
        TextureData(unsigned int width, unsigned int height, std::shared_ptr<unsigned char> dataPtr);
        TextureData(const TextureData& textureData);
        
        unsigned int getWidth() const { return width; }
        void setWidth(const unsigned int width) { this->width = width; }
        unsigned int getHeight() const { return height; }
        void setHeight(const unsigned int height) { this->height = height; }
        std::shared_ptr<unsigned char> getDataPtr() const { return dataPtr; };
        void setDataPtr(const std::shared_ptr<unsigned char> dataPtr) { this->dataPtr = dataPtr; };
    private:
        unsigned int width;
        unsigned int height;
        std::shared_ptr<unsigned char> dataPtr;
};

/*
 * TextureLoader handles buffering textures into OpenGL from file system or from texture data and maintaining a list of all
 * unique buffered textures by filePath and OpenGL name.
 */
class TextureLoader {
    friend Texture;
    public:
        /*
         * 
         */
        static void PreLoadTextures(const std::vector<std::string>& textureFilePaths);
        
        /*
         * Unloads all loaded textures with usage count less than 1 from system memory. Unloaded textures will need to be
         * loaded from system memory to be used again.
         */
        static void UnloadUnusedTextures();
        
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
        
        struct TextureInfo {
            std::string filePath;
            unsigned int height;
            unsigned int width;
            TextureDataPtr textureDataPtr;
            unsigned int textureName = 0;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<TextureInfo> loadedTextures;
};

}

#endif //TEXTURE_H
