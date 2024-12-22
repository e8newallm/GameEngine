#ifndef DATASTORE_H
#define DATASTORE_H

#include <iostream>
#include <map>
#include <string>

#ifdef DEBUG
#include <stdint.h>
#endif

template <class T> class Store : public std::map<std::string, T*>
{
    public:
        ~Store()
        {
            std::cout << "Deconstructing Store of " << typeid(T*).name() << "\r\n" << std::flush;
            for(std::pair<std::string, T*> value : *this)
            {
                std::cout << " - " << value.first << "\r\n";
                delete value.second;
            }
            std::cout << "Deconstructed Store of " << typeid(T*).name() << "\r\n" << std::flush;
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
        static inline Store<T> Data;
};

#endif