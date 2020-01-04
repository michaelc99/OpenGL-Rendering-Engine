#include "texture.h"

namespace Engine {

/*
 * Class Texture
 */
Texture::Texture(const std::string filePath, const TextureType type) : type(type) {
    this->textureID = TextureLoader::LoadTextureFromFile(filePath);
    TextureLoader::UseLoadedTexture(this->textureID);
}

Texture::Texture(const TextureDataPtr textureDataPtr, const TextureType type) : type(type) {
    this->textureID = TextureLoader::LoadTextureFromTextureData(textureDataPtr);
    TextureLoader::UseLoadedTexture(this->textureID);
}

Texture::Texture(const Texture& texture) : type(texture.type) {
    this->textureID = texture.textureID;
    TextureLoader::UseLoadedTexture(this->textureID);
}

Texture::~Texture() {
    TextureLoader::ReleaseLoadedTexture(this->textureID);
}

void Texture::bind() const {
    TextureLoader::BindTexture(this->textureID);
}

TextureDataPtr Texture::getTextureDataPtr() const {
    return TextureLoader::GetTextureDataPtr(this->textureID);
}

TextureDataPtr Texture::copyTextureData() const {
    return TextureLoader::CopyTextureDataFromLoaded(this->textureID);
}

}
