#include "mge/shader/program.hpp"
#
namespace mge {
namespace shader {
    program::program(program_type type)
        : m_type(type)
        , m_program_memory(std::pmr::get_default_resource())
        , m_source(char_allocator_type(&m_program_memory))
    {}

    program::~program()
    {}

    void
    program::set_source(const std::string &source)
    {
        m_source.assign(source.begin(), source.end());
        //char_allocator_type char_alloc(&m_program_memory);
        //m_source = new string_type(source.begin(), source.end(), char_alloc);

    }


}
}
