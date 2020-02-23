#pragma once
#include "mge/core/types.hpp"
#include <memory_resource>
#include <string>

namespace mge {
namespace shader {

    using char_allocator = std::pmr::polymorphic_allocator<char>;
    using string = std::basic_string<char, std::char_traits<char>, char_allocator>;
    using string_allocator = std::pmr::polymorphic_allocator<string>;


    class program;
    class module;
    class program_element;
    class statement;
    class main_function;
    class expression;
}
}
