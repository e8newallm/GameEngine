#include <string>

#include "graphics.h"
#include "image.h"
#include "graphics.h"
#include "texture.h"


Image::Image(SDL_Rect body, const std::string& texture) :
    Object(body, new Texture(texture))
{
}

ShaderObjData Image::predraw()
{
    struct ObjData {
        SDL_Rect body;
        SDL_FRect texBody;
    };

    ObjData* data = static_cast<ObjData*>(malloc(sizeof(ObjData)));
    data->body = body;
    data->texBody = tex->getUV();
    return {data, sizeof(ObjData)};
}