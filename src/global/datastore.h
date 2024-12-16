#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <string>

#ifdef DEBUG
#include <stdint.h>
#endif

template <class T, class storeID> class DataStore
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
        static std::map<std::string, uint64_t> getList()
        {
            std::map<std::string, uint64_t> list;
            for(std::pair<std::string, T*> name : Data)
            {
                list.insert({name.first, (uint64_t)name.second});
            }
            return list;
        }
#endif

    private:
        static inline std::map<std::string, T*> Data;
};

#endif