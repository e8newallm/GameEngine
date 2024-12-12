#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <string>

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

    private:
        static inline std::map<std::string, T*> Data;
};

#endif