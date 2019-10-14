#pragma once
#include <vector>
#include <array>
#include <variant>
#include <iterator>

namespace mge {

    namespace {
        template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
        template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
    }

    /**
     * A small vector to avoid allocation up an certain size of elements.
     * Note while this implements the vector interface, and works almost
     * like a normal vector, there are maybe not all standard guarantees
     * fulfilled.
     *
     * Especially, resize operations may copy way more elements e.g.
     * when shrinking the vector, if the implementation changes from
     * @c std::vector to the fixed size array.
     */
    template <typename T, size_t S, class Alloc = std::allocator<T> >
    class small_vector
    {
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    private:
        struct small_data 
        {
            small_data(size_t l, const value_type& val)
                :length(l)
            {
                for(size_t i=0; i<l; ++i) {
                    data[i] = val;
                }
            }

            explicit small_data(const small_data& d)
                :data(d.data)
                ,length(d.length)
            {}

            small_data(const_iterator b, const_iterator e)
                :length(0)
            {
                for (size_t i=0; b<e; ++i,++b) {
                    data[i] = *b;
                    ++length;
                }
            }

            explicit small_data()
                :length(0)
            {}

            std::array<T, S> data;
            size_t           length;
        };
    public:
        explicit small_vector()
        {}

        explicit small_vector(size_type size, const value_type& val = value_type())
        {
            if(size == 0) {
                // nothing
            } else if (size <= S) {
                m_data.emplace<1>(size, val);
            } else {
                m_data.emplace<2>(size, val);
            }
        }

        small_vector(const small_vector& v)
            :m_data(v.m_data)
            ,m_allocator(v.m_allocator)
        {}

        small_vector(small_vector&& v)
            :m_data(std::move(v.m_data))
            ,m_allocator(std::move(v.m_allocator))
        {}

        ~small_vector()
        {}


        small_vector& operator =(const small_vector& v)
        {
            m_data = v.m_data;
            return *this;
        }

        small_vector operator =(small_vector&& v)
        {
            m_data = std::move(v.m_data);
            return *this;
        }


        const_iterator begin() const 
        {
            return cbegin();
        }

        const_iterator cbegin() const 
        {
            return std::visit(
                overloaded {
                    [](const std::monostate&) -> const_iterator { return nullptr; },
                    [](const small_data& d) -> const_iterator { return d.data.data(); },
                    [](const std::vector<T, Alloc>& v) -> const_iterator { return v.data(); },
                }, 
                m_data);
        }

        iterator begin() 
        {
            return std::visit(
                overloaded {
                    [](std::monostate&) -> iterator { return nullptr; },
                    [](small_data& d) -> iterator { return d.data.data(); },
                    [](std::vector<T, Alloc>& v) -> iterator { return v.data(); },
                }, 
                m_data);
        }


        const_iterator end() const 
        {
            return cend();
        }

        const_iterator cend() const
        {
            return std::visit(
                overloaded {
                    [](const std::monostate&) -> const_iterator { return nullptr; },
                    [](const small_data& d) -> const_iterator { return d.data.data() + d.length; },
                    [](const std::vector<T, Alloc>& v) -> const_iterator { return v.data() + v.size(); },
                }, 
                m_data);
        }

        iterator end() 
        {
            return std::visit(
                overloaded {
                    [](std::monostate&) -> iterator { return nullptr; },
                    [](small_data& d) -> iterator { return d.data.data() + d.length; },
                    [](std::vector<T, Alloc>& v) -> iterator { return v.data() + v.size(); },
                }, 
                m_data);
        }

        const_reverse_iterator rbegin() const
        {
            return crbegin();
        }

        const_reverse_iterator crbegin() const 
        {
            return std::make_reverse_iterator(end());
        }

        const_reverse_iterator rend() const 
        {
            return crend();
        }

        const_reverse_iterator crend() const 
        {
            return std::make_reverse_iterator(begin());
        }

        size_type size() const
        {
            return std::visit(
                overloaded {
                    [](const std::monostate&) -> size_t { return 0; },
                    [](const small_data& d) -> size_t { return d.length; },
                    [](const std::vector<T, Alloc>& v) -> size_t { return v.size(); },
                }, 
                m_data);
        }

        size_type max_size() const
        {
            return m_allocator.max_size();
        }

        void resize(size_t n)
        {
            switch(m_data.index()) {
            case 0:
                resize_from_monostate(n);
                break;
            case 1:
                resize_from_small_data(n);
                break;
            case 2:
                resize_from_vector(n);
                break;
            }
        }

        void resize(size_t n, const value_type& fill)
        {
            switch(m_data.index()) {
            case 0:
                resize_from_monostate(n, fill);
                break;
            case 1:
                resize_from_small_data(n, fill);
                break;
            case 2:
                resize_from_vector(n, fill);
                break;
            }
        }

        // assign
        // at
        // operator []
        const_reference operator[](size_type i) const
        {
            return *(cbegin() + i);
        }

        reference operator[](size_type i)
        {
            return *(begin() + i);
        }

        // capacity
        // emplace
        // emplace_back

        // empty
        bool empty() const
        {
            return m_data.index() == 0;
        }


        // erase
        // front
        // get_allocator
        // insert
        // max_size
        // operator =
        // pop_back

        // push_back

        void push_back(const value_type& val)
        {
            switch(m_data.index()) {
            case 0:
                m_data.emplace<1>(1, val);
                break;
            case 1:
                push_back_small_data(val);
                break;
            case 2:
                std::get<2>(m_data).push_back(val);
                break;
            }
        }

        void push_back(value_type&& val)
        {
            switch(m_data.index()) {
            case 0:
                m_data.emplace<1>(small_data());
                std::get<1>(m_data).data[0] = std::move(val);
                std::get<1>(m_data).length = 1;
                break;
            case 1:
                push_back_small_data(std::move(val));
                break;
            case 2:
                std::get<2>(m_data).push_back(std::move(val));
                break;
            }
        }

        // reserve
        // shrink_to_fit
        // size
        // swap
    private:
        void push_back_small_data(const value_type& val)
        {
            if (std::get<1>(m_data).length == S) {
                convert_to_vector();
                std::get<2>(m_data).push_back(val);
            } else {
                std::get<1>(m_data).data[std::get<1>(m_data).length] = val;
                std::get<1>(m_data).length++;
            }
        }

        void push_back_small_data(value_type&& val)
        {
            if (std::get<1>(m_data).length == S) {
                convert_to_vector();
                std::get<2>(m_data).push_back(std::move(val));
            } else {
                std::get<1>(m_data).data[std::get<1>(m_data).length] = std::move(val);
                std::get<1>(m_data).length++;
            }
        }

        void resize_from_monostate(size_t n)
        {
            if (n == 0) {
                return;
            } else if (n < S) {
                m_data.emplace<1>(n, value_type());
            } else {
                m_data.emplace<2>(n, value_type());
            }
        }

        void resize_from_monostate(size_t n, const value_type& fill)
        {
            if (n == 0) {
                return;
            } else if (n < S) {
                m_data.emplace<1>(n, fill);
            } else {
                m_data.emplace<2>(n, fill);
            }
        }

        void resize_from_small_data(size_t n)
        {
            if (n == 0) {
                m_data.emplace<0>(std::monostate());
            } else if(n < std::get<1>(m_data).length) {
                for (size_t i=n; i<std::get<1>(m_data).length; ++i) {
                    std::get<1>(m_data).data[i] = value_type();
                }
                std::get<1>(m_data).length = n;
            } else if (n < S) {
                for (size_t i=std::get<1>(m_data).length; i<n; ++i) {
                    std::get<1>(m_data).data[i] = value_type();
                }
                std::get<1>(m_data).length = n;
            } else {
                convert_to_vector();
                std::get<2>(m_data).resize(n);
            }
        }

        void resize_from_small_data(size_t n, const value_type& fill)
        {
            if (n == 0) {
                m_data.emplace<0>(std::monostate());
            } else if(n < std::get<1>(m_data).length) {
                for (size_t i=n; i<std::get<1>(m_data).length; ++i) {
                    std::get<1>(m_data).data[i] = fill;
                }
                std::get<1>(m_data).length = n;
            } else if (n < S) {
                for (size_t i=std::get<1>(m_data).length; i<n; ++i) {
                    std::get<1>(m_data).data[i] = fill;
                }
                std::get<1>(m_data).length = n;
            } else {
                convert_to_vector();
                std::get<2>(m_data).resize(n, fill);
            }
        }


        void resize_from_vector(size_t n)
        {
            if(n == 0) {
                m_data.emplace<0>(std::monostate());
            } else if(n > S) {
                std::get<2>(m_data).resize(n);
            } else {
                m_data.emplace<1>(small_data(begin(), begin()+n));
            }
        }

        void resize_from_vector(size_t n, const value_type& fill)
        {
            if(n == 0) {
                m_data.emplace<0>(std::monostate());
            } else if(n > S) {
                std::get<2>(m_data).resize(n, fill);
            } else {
                m_data.emplace<1>(small_data(begin(), begin()+n));
            }
        }

        void convert_to_vector()
        {
            std::vector<value_type> v(begin(), end(), m_allocator);
            m_data.emplace<2>(v);
        }

        using data_type = std::variant<std::monostate,
                                       small_data,
                                       std::vector<T, Alloc>>;
        
        data_type m_data;
        Alloc     m_allocator;
    };
}
