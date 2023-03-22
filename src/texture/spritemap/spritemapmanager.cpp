#include "spritemapmanager.h"
#include <filesystem>

void SpriteMapManager::load(std::string fileLocation)
{
    std::filesystem::path file(fileLocation);
    SpriteMapData* data = new SpriteMapData();
    data->loadFromFile(rend, fileLocation.c_str());
    spriteMaps.insert(std::make_pair(file.filename().string(), data));
}

void SpriteMapManager::unload(std::string spriteMapName)
{

}

SpriteMapData& SpriteMapManager::operator[](std::string name)
{
    return *(spriteMaps.find(name)->second);
}