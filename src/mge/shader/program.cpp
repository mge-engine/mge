#include "mge/shader/program.hpp"
#include "mge/shader/module.hpp"

namespace mge {
namespace shader {
    program::program(program_type type)
        : m_type(type)
        , m_program_memory_base(std::pmr::get_default_resource())
        , m_program_memory(&m_program_memory_base)
        , m_modules(std::pmr::polymorphic_allocator<module>(&m_program_memory))
    {}

    program::~program()
    {}


    std::pmr::memory_resource *
    program::memory_resource()
    {
        return &m_program_memory;
    }

    void
    program::add_module(const std::string& name, const std::string& text)
    {
        module *m = create<module>(name, text);
        auto it = std::find_if(m_modules.begin(), m_modules.end(), [&](const module *e){
            if(e->name() == name) {
                return true;
            } else {
                return false;
            }
        });
        if (it != m_modules.end()) {
            m_modules.erase(it);
        }
        m_modules.push_back(m);
    }


}
}
