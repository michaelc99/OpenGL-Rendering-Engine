#include "texture_loader.h"

namespace Engine {

static unsigned int TextureLoader::loadTexture(std::string filePath) {
    for(size_t i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].filePath == filePath) {
            loadedTextures[i].count++;
            return loadedTextures[i].textureID;
        }
    }
    unsigned int textureID;
    // Load texture from file system into graphics memory
    
    TextureInfo textureInfo;
    textureInfo.filePath = filePath;
    textureInfo.textureID = textureID;
    textureInfo.count = 0;
    loadedTextures.push_back(textureInfo);
    
    return textureID;
}

static void TextureLoader::releaseTexture(std::string filePath) {
    for(size_t i = 0; i < loadedTextures.size(); i++) {
        if(loadedTextures[i].filePath == filePath) {
            loadedTextures[i].count--;
            break;
        }
    }
}

static void TextureLoader::unloadUnusedTextures() {
    for(std::vector<TextureInfo>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end(); iter++) {
        if(iter->count <= 0) {
            // Unload texture from graphics memory at textureID
            
            loadedTextures.erase(iter);
        }
    }
}

}
