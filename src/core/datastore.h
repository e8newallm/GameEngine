#ifndef DATASTORE_H
#define DATASTORE_H

#include <unordered_map>
#include <string>

#ifdef DEBUG
#include <stdint.h>

extern std::unordered_map<std::string, std::unordered_map<std::string, uint64_t> (*)()> storeList;
#endif

#include "geerror.h"

namespace GameEng
{
    /**
    * \brief A wrapper class around std::unordered_map that deletes all held pointer objects at deconstruction.
    *
    * \tparam T the type of object to be mapped.
    */
    template <class T> class Store : public std::unordered_map<std::string, T*>
    {
        public:
            ~Store()
            {
                for(std::pair<std::string, T*> value : *this)
                {
                    delete value.second;
                }
            }
    };

    template <typename T> consteval std::string_view getTypeName();

    /**
    * \brief Class for storing globally shared resources referenced using a name.
    *
    * \tparam T The class to be stored.
    * \tparam storeID The class the store will be contained in (used to guarantee a unique store).
    */
    template <class T, class storeID> class DataStore
    {
        public:

            /**
            * \brief Add a new entry to the store.
            * 
            * \param data A pointer to the object being added.
            * \param name A name to reference the object by.
            */
            static void add(T* data, const std::string& name)
            {
                Data.insert({name, data});
            }

            /**
            * \brief Retrieve an object from the store.
            * 
            * \param name The name of the object to be retrieved.
            * \return T* Pointer to the requested object.
            *
            * \throws GameEng::GEError::STORE_ENTRY_NOT_FOUND Thrown if there is no entry for the given name.
            */
            static T* get(const std::string& name)
            {
                if(exists(name))
                    return Data[name];
                throw GameEngineException(GEError::STORE_ENTRY_NOT_FOUND, "Requested entry " + name + " not found in " + ownerClass + " store");
            }

            /**
            * \brief Check whether there is an entry for a given name.
            *
            * \param name The name of the entry.
            * \return bool Does an entry exist for the given name?
            */
            static bool exists(const std::string& name)
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
            static inline constexpr std::string ownerClass = std::string(getTypeName<storeID>());
    };

    /**
    * \brief Get the name of Type T as a string_view.
    *
    * \tparam T the type to be turned into a string_view.
    * \return std::string_view the type's name as a string_view.
    */
    template <typename T> consteval std::string_view getTypeName()
    {
    #if defined(__clang__)
        constexpr auto prefix   = std::string_view{"[T = "};
        constexpr auto suffix   = std::string_view{";"};
        constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
    #elif defined(__GNUC__)
        constexpr auto prefix   = std::string_view{"with T = "};
        constexpr auto suffix   = std::string_view{";"};
        constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
    #elif defined(_MSC_VER)
        constexpr auto prefix   = std::string_view{"type_name_array<"};
        constexpr auto suffix   = std::string_view{">(void)"};
        constexpr auto function = std::string_view{__FUNCSIG__};
    #else
    #   error Unsupported compiler
    #endif

        constexpr auto start = function.find(prefix) + prefix.size();
        constexpr auto end = function.rfind(suffix);

        static_assert(start < end);

        constexpr static auto name = function.substr(start, (end - start));
        return name;
    }
}

#endif