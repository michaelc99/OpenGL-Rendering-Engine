#include "texture_data.h"

namespace Engine {

/*
 * Class TextureData
 */
TextureData::TextureData(const unsigned int width, const unsigned int height, const unsigned int numChannels, const std::shared_ptr<unsigned char[]> dataPtr)
    : width(width), height(height), numChannels(numChannels), dataPtr(dataPtr) {
    this->size = width * height * numChannels * bytesPerChannel;
    this->dataPtr = dataPtr;
}

TextureData::TextureData(const TextureData& textureData)
    : width(textureData.width), height(textureData.height), numChannels(textureData.numChannels) {
    this->size = textureData.size;
    this->dataPtr = std::shared_ptr<unsigned char[]>(new unsigned char[this->size]);
    memcpy(this->dataPtr.get(), textureData.dataPtr.get(), this->size);
}

/*
 * Class TextureLoader
 */
unsigned int TextureLoader::spareID = 1;
std::stack<unsigned int> TextureLoader::availableIDStack = std::stack<unsigned int>();
std::unordered_map<unsigned int, TextureLoader::TextureInfo> TextureLoader::loadedTextures = std::unordered_map<unsigned int, TextureLoader::TextureInfo>();

void TextureLoader::PreLoadTextures(const std::vector<std::string>& textureFilePaths) {
    for(unsigned int i = 0; i < textureFilePaths.size(); i++) {
        LoadTextureFromFile(textureFilePaths[i]);
    }
}

void TextureLoader::UnloadUnusedTextures() {
    for(std::unordered_map<unsigned int, TextureInfo>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end(); iter++) {
        if(iter->second.usingCount == 0) {
            UnloadTexture(iter->first);
        }
    }
}

void TextureLoader::BindTexture(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
#endif
    glBindTexture(GL_TEXTURE_2D, loadedTextures[textureID].textureName);
}

TextureDataPtr TextureLoader::GetTextureDataPtr(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
#endif
    return loadedTextures[textureID].textureDataPtr;
}

unsigned int TextureLoader::LoadTextureFromFile(const std::string filePath) {
    // Check if the texture is already buffered
    for(std::unordered_map<unsigned int, TextureInfo>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end(); iter++) {
        if(iter->second.filePath == filePath) {
            return iter->first;
        }
    }
    
    // Load texture from file system into memory
    TextureDataPtr textureDataPtr;
    int width = 0;
    int height = 0;
    int imgNumChannels = 0;
    stbi_set_flip_vertically_on_load(true);
    std::shared_ptr<unsigned char[]> dataPtr = std::shared_ptr<unsigned char[]>(stbi_load(filePath.c_str(), &width, &height, &imgNumChannels, 0));
    if(!dataPtr.get()) {
        throw Engine::FileIOException("ERROR: Failed to load image data from \"" + filePath + "\"");
    }
    textureDataPtr = std::make_shared<TextureData>((unsigned int)width, (unsigned int)height, (unsigned int)imgNumChannels, dataPtr);
    
    TextureInfo textureInfo;
    textureInfo.filePath = filePath;
    textureInfo.textureDataPtr = textureDataPtr;
    textureInfo.textureName = 0;
    textureInfo.usingCount = 0;
    if(availableIDStack.empty()) {
        availableIDStack.push(spareID++);
    }
    unsigned int textureID = availableIDStack.top();
    availableIDStack.pop();
    loadedTextures[textureID] = textureInfo;
    return textureID;
}

unsigned int TextureLoader::LoadTextureFromTextureData(const TextureDataPtr textureDataPtr) {
    TextureInfo textureInfo;
    textureInfo.filePath = "";
    textureInfo.textureDataPtr = std::make_shared<TextureData>(*(textureDataPtr.get()));
    textureInfo.textureName = 0;
    textureInfo.usingCount = 0;
    if(availableIDStack.empty()) {
        availableIDStack.push(spareID++);
    }
    unsigned int textureID = availableIDStack.top();
    availableIDStack.pop();
    loadedTextures[textureID] = textureInfo;
    return textureID;
}

//void TextureLoader::SaveTextureFromTextureData(const std::string& filePath, const TextureDataPtr textureDataPtr) {
//    
//}

void TextureLoader::UseLoadedTexture(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
#endif
    if(loadedTextures[textureID].usingCount == 0) {
        BufferTextureData(textureID);
    }
    loadedTextures[textureID].usingCount++;
}

void TextureLoader::ReleaseLoadedTexture(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
    assert(loadedTextures[textureID].usingCount > 0);
#endif
    loadedTextures[textureID].usingCount--;
    if(loadedTextures[textureID].usingCount == 0) {
        UnBufferTextureData(textureID);
        if(loadedTextures[textureID].filePath == "") {
            UnloadTexture(textureID);
        }
    }
}

TextureDataPtr TextureLoader::CopyTextureDataFromLoaded(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
#endif
    return std::make_shared<TextureData>(*(loadedTextures[textureID].textureDataPtr));
    
//    unsigned int pixelBufferID;
//    glGenBuffers(1, &pixelBufferID);
//    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
//    glBindTexture(GL_TEXTURE_2D, loadedTextures[textureID].textureName);
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    
//    int dataSize;
//    int expectedSize = loadedTextures[textureID].width * loadedTextures[textureID].height * 3;
//    glGetBufferParameteri64v(GL_PIXEL_PACK_BUFFER, GL_BUFFER_SIZE, &dataSize);
//    if(dataSize != expectedSize) {
//        throw TextureException("ERROR: Texture buffer data size of " + std::to_string(dataSize)
//            + " bytes, expected " + std::to_string(expectedSize) + " bytes.");
//    }
//    
//    unsigned char* bufferPtr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
//    std::shared_ptr<unsigned char> data = std::shared_ptr<unsigned char>(new unsigned char[dataSize]);
//    memcpy(data.get(), bufferPtr, dataSize);
//    textureDataPtr = std::make_shared<TextureData>(loadedTextures[textureID].width, loadedTextures[textureID].height, data);
//    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
//    return;
}

void TextureLoader::BufferTextureData(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
#endif
    TextureInfo textureInfo = loadedTextures[textureID];
    glGenTextures(1, &loadedTextures[textureID].textureName);
    glBindTexture(GL_TEXTURE_2D, loadedTextures[textureID].textureName);
    // Add ability to change settings for texture???????????
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureInfo.textureDataPtr->getWidth(), textureInfo.textureDataPtr->getHeight(),
            0, GL_RGB, GL_UNSIGNED_BYTE, textureInfo.textureDataPtr->getDataPtr().get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureLoader::UnBufferTextureData(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
#endif
    glDeleteTextures(1, &loadedTextures[textureID].textureName);
}

void TextureLoader::UnloadTexture(const unsigned int textureID) {
#ifdef _DEBUG
    assert(textureID != 0);
    assert(loadedTextures[textureID].usingCount == 0);
#endif
    for(std::unordered_map<unsigned int, TextureInfo>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end(); iter++) {
        if(iter->first == textureID) {
            availableIDStack.push(iter->first);
            loadedTextures.erase(iter);
            break;
        }
    }
}

}
