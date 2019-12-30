#ifndef TEXTURE_H
#define TEXTURE_H

#include <graphics/texture/texture_loader.h>
#include <memory>

namespace Engine {

enum TextureType {
    DIFFUSE,
    SPECULAR
};

class Texture {
    public:
        Texture(std::string filePath, TextureType type);
        ~Texture();
        
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
//        unsigned int width;
//        unsigned int height;
        
};

}

#endif //TEXTURE_H
