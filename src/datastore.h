#include <map>
#include <string>

template <class T> class DataStore
{
    public:
        static void add(T* data, std::string name)
        {
            Data.insert({name, data});
        }

        static T* get(std::string name)
        {
            return Data[name];
        }

        static bool exists(std::string name)
        {
            return Data.contains(name);
        }
        
    protected:
        static std::map<std::string, T*> Data;
};

//texture = SDL_CreateTextureFromSurface(rend, IMG_Load(texturePath));