#include "texture_base.h"
#include <memory>

namespace GameEng
{
    const std::shared_ptr<Texture_base> NoTexture = std::make_shared<TextureNothing>();
}