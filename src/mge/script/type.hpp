#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <type_traits>

namespace mge::script {

    namespace {
        template <typename T>
        concept is_integer =
            std::is_same_v<T, uint8_t> || std::is_same_v<T, int8_t> ||
            std::is_same_v<T, uint16_t> || std::is_same_v<T, int16_t> ||
            std::is_same_v<T, uint32_t> || std::is_same_v<T, int32_t> ||
            std::is_same_v<T, uint64_t> || std::is_same_v<T, int64_t>;

        template <typename T>
        concept is_floating_point =
            std::is_same_v<T, float> || std::is_same_v<T, double>;

        template <typename T>
        concept is_boolean = std::is_same_v<T, bool>;

        template <typename T>
        concept is_string =
            std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;

        template <typename T>
        concept is_enumeration = std::is_enum_v<T>;

        template <typename T>
        concept is_void = std::is_same_v<T, void>;

        // template <typename T>
        // concept is_class = std::is_class_v<T>;

        //  template <typename T>
        //  concept is_shared_ptr = std::is_same_v<T, std::shared_ptr<T>>;
    } // namespace

    // Simple Types
    // - integers
    // - floating point
    // - boolean
    // - string, wstring, ...
    // - enumerations
    // - void
    // Object Types
    // - classes
    // - shared_ptr<T>
    // Container Types
    // - vector<T>
    // - map<K,V>
    // - set<T>
    // - list<T>
    // - array<T,N>
    // - tuple<T...>
    // Function Type
    // - std::function
    // - function pointers ?
    // Uncreatable Types
    // - references to simple types, objects or containers
    // - pointers to simple types, objects or containers
    // Incompatible Types
    // - unique_ptr<T>
    // - containers of classes

    template <> class type<void>
    {};

} // namespace mge::script