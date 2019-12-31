#ifndef TEXTURE_H
#define TEXTURE_H

#include <graphics/images/image_loader.h>
#include <exceptions/render_exception.h>
#include <exceptions/io_exception.h>
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
         * 
         */
        void copyTextureData(const TextureDataPtr textureDataPtr);
        
        std::string getFilePath() const { return filePath; }
        void setFilePath(std::string filePath) { this->filePath = filePath; }
        TextureType getType() const { return type; }
        void setType(TextureType type) { this->type = type; }
        unsigned int getTextureID() const { return textureID; }
        void setTextureID(unsigned int textureID) { this->textureID = textureID; }
    private:
        std::string filePath;
        TextureType type;
        unsigned int textureID;
        unsigned int width;
        unsigned int height;
};

/*
 * TextureData contains the pixel data for a texture in system memory.
 */
class TextureData {
    public:
        TextureData();
        TextureData(unsigned int width, unsigned int height, std::shared_ptr<unsigned char> data);
        unsigned int getWidth() const { return width; }
        void setWidth(const unsigned int width) { this->width = width; }
        unsigned int getHeight() const { return height; }
        void setHeight(const unsigned int height) { this->height = height; }
        std::shared_ptr<unsigned char> getData() const { return data; };
        void setData(const std::shared_ptr<unsigned char> data) { this->data = data; };
    private:
        unsigned int width;
        unsigned int height;
        std::shared_ptr<unsigned char> data;
};

/*
 * TextureLoader handles buffering textures into OpenGL from file system or from texture data and maintaining a list of all
 * unique buffered textures by filePath and OpenGL name.
 */
class TextureLoader {
    friend Texture;
    private:
        /*
         * Loads texture and buffers it using OpenGL. Returns OpenGL name of texture;
         * If a texture with the same filename is already buffered, then the buffered copy will be used. If the texture is not
         * currently buffered then it will be loaded from the file system.
         */
        static unsigned int loadTextureFromFile(const std::string filePath, unsigned int& width, unsigned int& height);
        
        /*
         * Increments using count for texture with OpenGL name of textureID. Returns OpenGL name of texture;
         */
        static unsigned int useTextureFromLoaded(const unsigned int textureID);
        
        /*
         * Creates an OpenGL readable texture from TextureData and buffers it using OpenGL. Returns OpenGL name of texture;
         */
        static unsigned int generateTextureFromData(const TextureDataPtr textureDataPtr);
        
        /*
         * Buffers texture data from TextureDataPtr using OpenGL. Returns OpenGL name of texture;
         */
        static unsigned int bufferTextureData(const TextureDataPtr textureDataPtr);
        
        /*
         * Copies the buffered texture data from textureID to TextureDataPtr.
         */
        static void copyTextureDataFromLoaded(const unsigned int textureID, const TextureDataPtr textureDataPtr);
        
        /*
         * Decrements using count for texture with OpenGL name of textureID. Returns OpenGL name of texture.
         */
        static void releaseTexture(const unsigned int textureID);
        
        /*
         * Deletes/unbuffers all buffered textures with usage count less than 1.
         */
        static void unloadUnusedTextures();
        
        struct TextureInfo {
            std::string filePath;
            unsigned int height;
            unsigned int width;
            unsigned int textureID;
            unsigned int count;
        };
        static std::vector<TextureInfo> loadedTextures;
};

}

#endif //TEXTURE_H
