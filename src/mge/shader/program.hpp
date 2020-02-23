#pragma once
#include "mge/core/types.hpp"
#include "mge/shader/dllexport.hpp"
#include "mge/shader/shader_fwd.hpp"
#include "mge/shader/program_type.hpp"
#include "mge/core/memory_resource.hpp"
#include <string_view>
#include <type_traits>

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

        template <typename T, typename... ArgTypes>
        T* create(ArgTypes&&...args)
        {
            void *ptr = m_program_memory.allocate(sizeof(T), alignof(T));
            try {
                if constexpr (std::is_constructible<T, program *, ArgTypes...>::value) {
                    return new (ptr) T(this, std::forward< ArgTypes >( args )...);
                } else {
                    return new (ptr) T(std::forward< ArgTypes >( args )...);
                }
            } catch(...) {
                m_program_memory.deallocate(ptr, sizeof(T), alignof(T));
                throw;
            }
        }

        void add_module(const std::string& name, const std::string& text);

        std::pmr::memory_resource *memory_resource();

    private:
        typedef std::pmr::vector<module *> module_vector;


        program_type                            m_type;
        main_function                          *m_main;
        mge::callback_memory_resource           m_program_memory_base;
        std::pmr::unsynchronized_pool_resource  m_program_memory;
        module_vector                           m_modules;
    };

}
}
