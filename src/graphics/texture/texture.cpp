#include "texture.h"

namespace Engine {

Texture::Texture(const std::string filePath, const TextureType type) : filePath(filePath), type(type) {
    this->textureID = TextureLoader::loadTextureFromFile(filePath, width, height);
}

Texture::Texture(const TextureDataPtr textureDataPtr, const TextureType type) : filePath(""), type(type) {
    this->textureID = TextureLoader::generateTextureFromData(textureDataPtr);
    this->width = textureDataPtr->getWidth();
    this->height = textureDataPtr->getHeight();
}

Texture::Texture(const Texture& texture) : filePath(texture.filePath), type(texture.type), width(texture.width), height(texture.height) {
    this->textureID = TextureLoader::useTextureFromLoaded(texture.textureID);
}

Texture::~Texture() {
    TextureLoader::releaseTexture(textureID);
}

void Texture::copyTextureData(const TextureDataPtr textureDataPtr) {
    if(textureID == 0) {
        throw TextureException("ERROR: Attempted to copy texture data from uninitialized texture.");
    }
    TextureLoader::copyTextureDataFromLoaded(textureID, textureDataPtr);
}

TextureData::TextureData() : width(0), height(0) {}

TextureData::TextureData(const unsigned int width, const unsigned int height, const std::shared_ptr<unsigned char> data)
    : width(width), height(height), data(data) {}

unsigned int TextureLoader::loadTextureFromFile(const std::string filePath, unsigned int& width, unsigned int& height) {
    // Check if the texture is already buffered
    for(size_t i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].filePath == filePath) {
            loadedTextures[i].count++;
            return loadedTextures[i].textureID;
        }
    }
    
    // Load texture from file system into graphics memory
    TextureDataPtr textureDataPtr;
    int imgNumChannels = 0;
    stbi_set_flip_vertically_on_load(true);
    textureDataPtr->setData(std::shared_ptr<unsigned char>(stbi_load(filePath.c_str(), &(int)width, &(int)height, &imgNumChannels, 0)));
    if(!textureDataPtr->getData().get()) {
        throw Engine::FileIOException("ERROR: Failed to load image data from \"" + filePath + "\"");
    }
    unsigned int textureID = bufferTextureData(textureDataPtr);
    
    TextureInfo textureInfo;
    textureInfo.filePath = filePath;
    textureInfo.textureID = textureID;
    textureInfo.count = 0;
    loadedTextures.push_back(textureInfo);
    return textureID;
}

unsigned int TextureLoader::useTextureFromLoaded(const unsigned int textureID) {
    for(size_t i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].textureID == textureID) {
            loadedTextures[i].count++;
            break;
        }
    }
    return textureID;
}

unsigned int TextureLoader::generateTextureFromData(const TextureDataPtr textureDataPtr) {
    unsigned int textureID = bufferTextureData(textureDataPtr);
    TextureInfo textureInfo;
    textureInfo.filePath = "";
    textureInfo.textureID = textureID;
    textureInfo.count = 0;
    loadedTextures.push_back(textureInfo);
    return textureID;
}

unsigned int TextureLoader::bufferTextureData(const TextureDataPtr textureDataPtr) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Add ability to change settings for texture???????????
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureDataPtr->getWidth(), textureDataPtr->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureDataPtr->getData().get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void TextureLoader::copyTextureDataFromLoaded(const unsigned int textureID, const TextureDataPtr textureDataPtr) {
    for(size_t i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].textureID == textureID) {
            unsigned int pixelBufferID;
            glGenBuffers(1, &pixelBufferID);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
            glBindTexture(GL_TEXTURE_2D, loadedTextures[i].textureID);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            int dataSize;
            int expectedSize = loadedTextures[i].width * loadedTextures[i].height * 3;
            glGetBufferParameteri64v(GL_PIXEL_PACK_BUFFER, GL_BUFFER_SIZE, &dataSize);
            if(dataSize != expectedSize) {
                throw TextureException("ERROR: Texture buffer data size of " + std::to_string(dataSize) + " bytes, expected " + std::to_string(expectedSize) + " bytes.");
            }
            
            unsigned char* bufferPtr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
            std::shared_ptr<unsigned char> data = std::shared_ptr<unsigned char>(new unsigned char[dataSize]);
            memcpy(data.get(), bufferPtr, dataSize);
            textureDataPtr = std::make_shared<TextureData>(loadedTextures[i].width, loadedTextures[i].height, data);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            return;
        }
    }
    throw TextureException("ERROR: Attempted to read texture data from texture that isn't loaded.");
}

void TextureLoader::releaseTexture(const unsigned int textureID) {
    for(size_t i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].textureID == textureID) {
            loadedTextures[i].count--;
            break;
        }
    }
}

void TextureLoader::unloadUnusedTextures() {
    for(std::vector<TextureInfo>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end(); iter++) {
        if(iter->count <= 0) {
            // Unload texture from graphics memory at textureID
            glDeleteTextures(1, &(iter->textureID));
            loadedTextures.erase(iter);
        }
    }
}

}
