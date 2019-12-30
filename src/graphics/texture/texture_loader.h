#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <graphics/texture/texture.h>
#include <string>
#include <vector>

namespace Engine {

class TextureLoader {
    public:
        static unsigned int loadTexture(std::string filePath);
        static void releaseTexture(std::string filePath);
        static void unloadUnusedTextures();
    private:
        struct TextureInfo {
            std::string filePath;
            unsigned int textureID;
            unsigned int count;
        };
        static std::vector<TextureInfo> loadedTextures;
};

}

#endif //TEXTURE_LOADER_H
