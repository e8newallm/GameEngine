#include <memory>
#include <string>

#include "image.h"
#include "texture.h"

namespace GameEng
{
Image::Image(SDL_Rect body, const std::string& texture) : Object(body, std::make_shared<Texture>(texture))
{
}

std::vector<std::byte> Image::predraw()
{
    struct ObjData
    {
        SDL_Rect body;
        SDL_FRect texBody;
    };

    std::vector<std::byte> mem(sizeof(ObjData));
    ObjData* data = reinterpret_cast<ObjData*>(mem.data());
    data->body = body;
    data->texBody = tex->getUV();
    return mem;
}
} // namespace GameEng