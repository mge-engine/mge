#pragma once
#include "mge/core/types.hpp"
#include "mge/shader/dllexport.hpp"
#include "mge/shader/program_type.hpp"
#include "mge/core/memory_resource.hpp"
#include <memory_resource>
#include <string_view>
#include <string>

namespace mge {
namespace shader {

    /**
     * A shader program.
     */
    class MGE_SHADER_EXPORT program
            : mge::noncopyable
    {
    public:
        program(program_type type);
        ~program();
        program_type type() const noexcept { return m_type; }

        void set_source(const std::string& source);

//        template <typename T, typename... ArgTypes>
//        T* create(ArgTypes&&...args) noexcept(std::is_nothrow_constructible<T, ArgTypes...>::value)
//        {
//            void *ptr = m_program_memory.allocate(sizeof(T), alignof(T));
//            new (ptr) T(std::forward< ArgTypes >( args )...);
//            return ptr;
//        }

        std::string_view source() const;
    private:
        using char_allocator_type = std::pmr::polymorphic_allocator<char>;
        using string_type = std::basic_string<char, std::char_traits<char>, char_allocator_type>;
        using string_allocator_type = std::pmr::polymorphic_allocator<string_type>;

        program_type m_type;
        mge::tracing_memory_resource m_program_memory;
        string_type m_source;
    };

}
}
