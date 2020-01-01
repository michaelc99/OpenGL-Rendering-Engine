#include "texture.h"

namespace Engine {

/*
 * Class Texture
 */
Texture::Texture(const std::string filePath, const TextureType type) : filePath(filePath), type(type) {
    this->textureID = TextureLoader::LoadTextureFromFile(filePath, width, height);
    TextureLoader::UseLoadedTexture(this->textureID);
}

Texture::Texture(const TextureDataPtr textureDataPtr, const TextureType type) : filePath(""), type(type) {
    this->textureID = TextureLoader::LoadTextureFromTextureData(textureDataPtr);
    this->width = textureDataPtr->getWidth();
    this->height = textureDataPtr->getHeight();
    TextureLoader::UseLoadedTexture(this->textureID);
}

Texture::Texture(const Texture& texture) : filePath(texture.filePath), type(texture.type), width(texture.width), height(texture.height) {
    this->textureID = texture.textureID;
    TextureLoader::UseLoadedTexture(this->textureID);
}

Texture::~Texture() {
    TextureLoader::ReleaseLoadedTexture(this->textureID);
}

TextureDataPtr Texture::copyTextureData() {
    TextureDataPtr textureDataPtr;
    TextureLoader::CopyTextureDataFromLoaded(this->textureID, textureDataPtr);
    return textureDataPtr;
}

/*
 * Class TextureData
 */
TextureData::TextureData() : width(0), height(0) {}

TextureData::TextureData(const unsigned int width, const unsigned int height, const std::shared_ptr<unsigned char> dataPtr)
    : width(width), height(height), dataPtr(dataPtr) {}

TextureData::TextureData(const TextureData& textureData) : width(textureData.getWidth()), height(textureData.getHeight()) {
    this->dataPtr = std::shared_ptr<unsigned char>(new unsigned char[sizeof(textureData.getDataPtr().get())]);
    memcpy(this->dataPtr.get(), textureData.getDataPtr().get(), sizeof(textureData.getDataPtr().get()));
}

/*
 * Class TextureLoader
 */
void TextureLoader::PreLoadTextures(const std::vector<std::string>& textureFilePaths) {
    for(int i = 0; i < textureFilePaths.size(); i++) {
        unsigned int width;
        unsigned int height;
        LoadTextureFromFile(textureFilePaths[i], width, height);
    }
}

void TextureLoader::UnloadUnusedTextures() {
    for(int i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].usingCount == 0) {
            UnloadTexture(i);
        }
    }
}

int TextureLoader::LoadTextureFromFile(const std::string filePath, unsigned int& width, unsigned int& height) {
    // Check if the texture is already buffered
    for(unsigned int index = 0; index < loadedTextures.size(); index++) {
        if(loadedTextures[index].filePath == filePath) {
            width = loadedTextures[index].width;
            height = loadedTextures[index].height;
            return index;
        }
    }
    
    // Load texture from file system into memory
    TextureDataPtr textureDataPtr;
    int imgNumChannels = 0;
    stbi_set_flip_vertically_on_load(true);
    std::shared_ptr<unsigned char> dataPtr = std::shared_ptr<unsigned char>(stbi_load(filePath.c_str(), &(int)width, &(int)height, &imgNumChannels, 0));
    if(!dataPtr.get()) {
        throw Engine::FileIOException("ERROR: Failed to load image data from \"" + filePath + "\"");
    }
    textureDataPtr = std::make_shared<TextureData>(width, height, dataPtr);
    
    TextureInfo textureInfo;
    textureInfo.filePath = filePath;
    textureInfo.width = width;
    textureInfo.height = height;
    textureInfo.textureDataPtr = textureDataPtr;
    textureInfo.textureName = 0;
    textureInfo.usingCount = 0;
    loadedTextures.push_back(textureInfo);
    
    int textureID = loadedTextures.size() - 1;
    return textureID;
}

int TextureLoader::LoadTextureFromTextureData(const TextureDataPtr textureDataPtr) {
    TextureInfo textureInfo;
    textureInfo.filePath = "";
    textureInfo.width = textureDataPtr->getWidth();
    textureInfo.height = textureDataPtr->getHeight();
    textureInfo.textureDataPtr = textureDataPtr;
    textureInfo.textureName = 0;
    textureInfo.usingCount = 0;
    loadedTextures.push_back(textureInfo);
    
    int textureID = loadedTextures.size() - 1;
    return textureID;
}

void TextureLoader::UseLoadedTexture(const int textureID) {
#ifdef _DEBUG
    assert(textureID != -1);
    assert(textureID < loadedTextures.size());
#endif
    if(loadedTextures[textureID].usingCount == 0) {
        BufferTextureData(textureID);
    }
    loadedTextures[textureID].usingCount++;
}

void TextureLoader::ReleaseLoadedTexture(const int textureID) {
#ifdef _DEBUG
    assert(textureID != -1);
    assert(textureID < loadedTextures.size());
    assert(loadedTextures[textureID].usingCount > 0);
#endif
    loadedTextures[textureID].usingCount--;
    if(loadedTextures[textureID].usingCount == 0) {
        UnbufferTextureData(textureID);
    }
}

void TextureLoader::CopyTextureDataFromLoaded(const int textureID, const TextureDataPtr textureDataPtr) {
#ifdef _DEBUG
    assert(textureID != -1);
    assert(textureID < loadedTextures.size());
#endif
    *textureDataPtr = TextureData(*(loadedTextures[textureID].textureDataPtr));
    
    /*unsigned int pixelBufferID;
    glGenBuffers(1, &pixelBufferID);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glBindTexture(GL_TEXTURE_2D, loadedTextures[textureID].textureName);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    int dataSize;
    int expectedSize = loadedTextures[textureID].width * loadedTextures[textureID].height * 3;
    glGetBufferParameteri64v(GL_PIXEL_PACK_BUFFER, GL_BUFFER_SIZE, &dataSize);
    if(dataSize != expectedSize) {
        throw TextureException("ERROR: Texture buffer data size of " + std::to_string(dataSize)
            + " bytes, expected " + std::to_string(expectedSize) + " bytes.");
    }
    
    unsigned char* bufferPtr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    std::shared_ptr<unsigned char> data = std::shared_ptr<unsigned char>(new unsigned char[dataSize]);
    memcpy(data.get(), bufferPtr, dataSize);
    textureDataPtr = std::make_shared<TextureData>(loadedTextures[textureID].width, loadedTextures[textureID].height, data);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    return;*/
}

void TextureLoader::BufferTextureData(const int textureID) {
#ifdef _DEBUG
    assert(textureID != -1);
    assert(textureID < loadedTextures.size());
#endif
    glGenTextures(1, &loadedTextures[textureID].textureName);
    glBindTexture(GL_TEXTURE_2D, loadedTextures[textureID].textureName);
    // Add ability to change settings for texture???????????
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loadedTextures[textureID].textureDataPtr->getWidth(), loadedTextures[textureID].textureDataPtr->getHeight(),
            0, GL_RGB, GL_UNSIGNED_BYTE, loadedTextures[textureID].textureDataPtr->getDataPtr().get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureLoader::UnbufferTextureData(const int textureID) {
#ifdef _DEBUG
    assert(textureID != -1);
    assert(textureID < loadedTextures.size());
#endif
    glDeleteTextures(1, &loadedTextures[textureID].textureName);
}

void TextureLoader::UnloadTexture(const int textureID) {
#ifdef _DEBUG
    assert(textureID != -1);
    assert(textureID < loadedTextures.size());
    assert(loadedTextures[textureID].usingCount == 0);
#endif
    int index = 0;
    for(std::vector<TextureInfo>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end(); iter++) {
        if(index == textureID) {
            loadedTextures.erase(iter);
            break;
        }
        index++;
    }
}

}
