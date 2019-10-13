#pragma once
#include <vector>
#include <array>
#include <variant>
namespace mge {
#if 0    
    template <typename T, size_t S, class Alloc = std::allocator<T> >
    class small_vector
    {
    public:
        small_vector()
        {}

    private:
        struct small_data {
            std::array<T, S> data;
            size_t           length;
        };

        using data_type = std::variant<std::monostate,
                                       small_data,
                                       std::vector<T, Alloc>>;
        
        data_type m_data;
    };
#endif
}