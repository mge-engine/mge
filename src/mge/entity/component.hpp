#pragma once
#include "mge/core/tagged.hpp"
namespace mge {

#define MGE_ENTITY_COMPONENT(type, name)                        \
    struct name##_tag {};                                       \
    using name = mge::tagged<type, name##_tag>;                 \
                                                                \
    template <typename... ArgTypes,                             \
              typename = std::enable_if_t<std::is_constructible<type, ArgTypes...>::value > > \
    void set_##name(ArgTypes&& ... args)                        \
    {                                                           \
        registry().assign_or_replace<name>(entity(), std::forward< ArgTypes>( args )...); \
    }                                                           \
                                                                \
    auto get_##name() const                                     \
    {                                                           \
        return registry().get<name>(entity()).get();            \
    }                                                           \
                                                                \
    void clear_##name()                                         \
    {                                                           \
        registry().remove<name>(entity());                      \
    }

#define MGE_ENTITY_BOOLEAN_COMPONENT(type, name)                \
    struct name {};                                             \
                                                                \
    void set_##name(bool value)                                 \
    {                                                           \
        if(value) {                                             \
            registry().assign_or_replace<name>(entity());       \
        } else {                                                \
            registry().remove<name>(entity());                  \
        }                                                       \
    }                                                           \
                                                                \
    bool get_##name() const                                     \
    {                                                           \
        return registry().has<name>(entity());                  \
    }                                                           \
                                                                \
    void clear_##name()                                         \
    {                                                           \
        registry().remove<name>(entity());                      \
    }


}

