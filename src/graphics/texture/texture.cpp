#include "texture.h"

namespace Engine {

Texture::Texture(std::string filePath, TextureType type) : filePath(filePath), type(type) {
    textureID = TextureLoader::loadTexture(filePath);
}

Texture::~Texture() {
    TextureLoader::releaseTexture(filePath);
}

}
