#ifndef DATASTORE_H
#define DATASTORE_H

#ifdef DEBUG
#include <stdint.h>
#endif

#include <unordered_map>
#include <string>

#include "logging.h"

template <class T> class Store : public std::unordered_map<std::string, T*>
{
    public:
        ~Store()
        {
            Logger::debug(std::string("Deconstructing Store of ") + typeid(T*).name());
            for(std::pair<std::string, T*> value : *this)
            {
                Logger::debug(std::string(" - ") + value.first);
                delete value.second;
            }
            Logger::debug(std::string("Deconstructed Store of ") + typeid(T*).name());
        }
};

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
        static std::unordered_map<std::string, uint64_t> getList()
        {
            std::unordered_map<std::string, uint64_t> list;
            for(std::pair<std::string, T*> name : Data)
            {
                list.insert({name.first, (uint64_t)name.second});
            }
            return list;
        }
#endif

    private:
        static inline Store<T> Data;
};

#endif