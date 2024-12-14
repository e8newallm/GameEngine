#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <string>

#ifdef DEBUG
#include <vector>
#endif

template <class T, class> class DataStore
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

#ifdef DEBUG
        static std::vector<std::string> getList()
        {
            std::vector<std::string> list;
            for(std::pair<std::string, T*> name : Data)
            {
                list.push_back(name);
            }

            return list;
        }
#endif

    private:
        static inline std::map<std::string, T*> Data;
};

#endif