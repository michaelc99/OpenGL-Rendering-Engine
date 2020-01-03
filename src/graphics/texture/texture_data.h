#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include <exceptions/io_exception.h>
#include <fileio/image_reader.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>

#include <glad/glad.h>

namespace Engine {

/*
 * TextureData contains the pixel data for a texture in system memory.
 */
class TextureData {
    public:
        TextureData(const unsigned int width, const unsigned int height, const unsigned int numChannels, const std::shared_ptr<unsigned char> dataPtr);
        TextureData(const TextureData& textureData);
        
        unsigned int getWidth() const { return width; }
        void setWidth(const unsigned int width) { this->width = width; }
        unsigned int getHeight() const { return height; }
        void setHeight(const unsigned int height) { this->height = height; }
        unsigned int getNumChannels() const { return numChannels; }
        void setNumChannels(const unsigned int numChannels) { this->numChannels = numChannels; }
        std::shared_ptr<unsigned char> getDataPtr() const { return dataPtr; };
        void setDataPtr(const std::shared_ptr<unsigned char> dataPtr) { this->dataPtr = dataPtr; };
        
        /*std::string getFormat() { return format; }
        void setFormat(const std::string& format) { this->format = format; }*/
    private:
        unsigned int width;
        unsigned int height;
        unsigned int numChannels;
        const unsigned short bytesPerChannel = 1;
        unsigned int size;
        std::shared_ptr<unsigned char> dataPtr;
        /*std::string format;
        std::string type;*/
};
typedef std::shared_ptr<TextureData> TextureDataPtr;

/*
 * TextureLoader handles loading textures from file system or from texture data into system memory and buffering textures
 * into OpenGL. TextureLoader maintains a list of all loaded textures.
 */
class TextureLoader {
    public:
        /*
         * Loads textures from list of file paths into system memory.
         */
        static void PreLoadTextures(const std::vector<std::string>& textureFilePaths);
        
        /*
         * Unloads all loaded textures with usage count less than 1 from system memory.
         */
        static void UnloadUnusedTextures();
        
        /*
         * Returns a TextureData pointer to texture data with index textureID from list of loaded textures.
         */
        static TextureDataPtr GetTextureDataPtr(const int textureID);
        
        /*
         * Binds texture about to be rendered.
         */
        static void BindTexture(const int textureID);
        
        /*
         * Loads texture from file system into system memory. Returns the index of texture from list of loaded textures.
         * If a texture with the same filename is already loaded, then the loaded instance will be used. If the texture is
         * not currently loaded then it will be loaded from the file system.
         */
        static int LoadTextureFromFile(const std::string filePath);
        
        /*
         * Puts texture with data given by TextureDataPtr into list of loaded textures. Returns the index of the texture
         * from list of loaded textures.
         */
        static int LoadTextureFromTextureData(const TextureDataPtr textureDataPtr);
        
        /*
         * Saves textureDataPtr to image file in file system.
         */
        static void SaveTextureFromTextureData(const std::string& filePath, const TextureDataPtr textureDataPtr);
        
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
         * Returns a deep copy of the loaded texture data with index textureID from list of loaded textures.
         */
        static TextureDataPtr CopyTextureDataFromLoaded(const int textureID);
    private:
        /*
         * Buffers texture data to GPU from loaded texture list with index textureID.
         */
        static void BufferTextureData(const int textureID);
        
        /*
         * Deletes OpenGL buffer of texture with index textureID from loaded texture list.
         */
        static void UnBufferTextureData(const int textureID);
        
        /*
         * Unloads loaded texture from system memory. Removes the texture from list of loaded textures.
         */
        static void UnloadTexture(const int textureID);
        
        struct TextureInfo {
            std::string filePath;
            TextureDataPtr textureDataPtr;
            unsigned int textureName = 0;
            unsigned int usingCount = 0;
        };
        // CHANGE TO SINGLETON PATTERN TO ALLOW RESEARTING OF ENGINE!!!!!!!!!!!!
        static std::vector<TextureInfo> loadedTextures;
};

}

#endif //TEXTURE_DATA_H
