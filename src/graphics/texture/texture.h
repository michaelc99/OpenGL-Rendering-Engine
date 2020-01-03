#ifndef TEXTURE_H
#define TEXTURE_H

#include <graphics/texture/texture_data.h>
#include <vector>
#include <string>

namespace Engine {

enum TextureType {
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR
};

class Texture {
    public:
        Texture(const std::string filePath, const TextureType type);
        Texture(const TextureDataPtr textureDataPtr, const TextureType type);
        Texture(const Texture& texture);
        ~Texture();
        
        /*
         * Binds the texture for use.
         */
        void apply() const;
        
        /*
         * Returns a TextureDataPtr to a shallow copy of the texture's data in the list of (shared) loaded textures.
         */
        TextureDataPtr getTextureDataPtr() const;
        
        /*
         * Returns TextureDataPtr to a deep copy of this texture's data.
         */
        TextureDataPtr copyTextureData() const;
        
        TextureType getType() const { return type; }
        void setType(const TextureType type) { this->type = type; }
        unsigned int getWidth() const { return getTextureDataPtr()->getWidth(); }
        unsigned int getHeight() const { return getTextureDataPtr()->getHeight(); }
    private:
        int textureID = -1;
        TextureType type;
};

}

#endif //TEXTURE_H
