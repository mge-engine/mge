#include "mge/shader/program_element.hpp"
namespace mge {
namespace shader {

    program_element::program_element()
    {}

    program_element::program_element(const location &l)
        :m_source_location(l)
    {}

    program_element::program_element(const program_element& el)
        :m_source_location(el.m_source_location)
    {}

}
}
