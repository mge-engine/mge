// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/overloaded.hpp"
#include "mge/core/stdexceptions.hpp"
#include <array>
#include <functional>
#include <iterator>
#include <limits>
#include <variant>
#include <vector>

#ifdef max
#    undef max
#endif

namespace mge {

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
    template <typename T, size_t S, class Alloc = std::allocator<T>>
    class small_vector
    {
    public:
        using value_type = T;
        using allocator_type = Alloc;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator = T*;
        using const_iterator = const T*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        struct small_data
        {
#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
            small_data(size_t l, const value_type& val)
                : length(l)
            {
                for (size_t i = 0; i < l; ++i) {
                    data[i] = val;
                }
            }

            small_data(const small_data& d)
                : data(d.data)
                , length(d.length)
            {}

            small_data& operator=(const small_data& d)
            {
                data = d.data;
                length = d.length;
                return *this;
            }

            small_data(const_iterator b, const_iterator e)
                : length(0)
            {
                for (size_t i = 0; b < e; ++i, ++b) {
                    data[i] = *b;
                    ++length;
                }
            }

            explicit small_data()
                : length(0)
            {}
#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic pop
#endif

            template <class... Args> T& emplace_back(Args&&... args)
            {
                new (&data[length]) T(std::forward<Args>(args)...);
                return data[length++];
            }

#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
            std::array<T, S> data;
#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic pop
#endif
            size_t length;
        };

    public:
        explicit small_vector() {}

        explicit small_vector(size_type         size,
                              const value_type& val = value_type())
        {
            if (size == 0) {
                // nothing
            } else if (size <= S) {
                m_data.template emplace<1>(size, val);
            } else {
                m_data.template emplace<2>(size, val);
            }
        }

        small_vector(std::initializer_list<T> init)
        {
            if (init.size() == 0) {
                // nothing
            } else if (init.size() <= S) {
                m_data.template emplace<1>(small_data());
                for (const auto& val : init) {
                    std::get<1>(m_data).emplace_back(val);
                }
            } else {
                m_data.template emplace<2>(init);
            }
        }

        small_vector(const small_vector& v)
            : m_data(v.m_data)
            , m_allocator(v.m_allocator)
        {}

        small_vector(small_vector&& v)
            : m_data(std::move(v.m_data))
            , m_allocator(std::move(v.m_allocator))
        {}

        ~small_vector() {}

        small_vector& operator=(const small_vector& v)
        {
            m_data = v.m_data;
            return *this;
        }

        small_vector operator=(small_vector&& v)
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
                overloaded{
                    [](const std::monostate&) -> const_iterator {
                        return nullptr;
                    },
                    [](const small_data& d) -> const_iterator {
                        return d.data.data();
                    },
                    [](const std::vector<T, Alloc>& v) -> const_iterator {
                        return v.data();
                    },
                },
                m_data);
        }

        iterator begin()
        {
            return std::visit(
                overloaded{
                    [](std::monostate&) -> iterator { return nullptr; },
                    [](small_data& d) -> iterator { return d.data.data(); },
                    [](std::vector<T, Alloc>& v) -> iterator {
                        return v.data();
                    },
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
                overloaded{
                    [](const std::monostate&) -> const_iterator {
                        return nullptr;
                    },
                    [](const small_data& d) -> const_iterator {
                        return d.data.data() + d.length;
                    },
                    [](const std::vector<T, Alloc>& v) -> const_iterator {
                        return v.data() + v.size();
                    },
                },
                m_data);
        }

        iterator end()
        {
            return std::visit(
                overloaded{
                    [](std::monostate&) -> iterator { return nullptr; },
                    [](small_data& d) -> iterator {
                        return d.data.data() + d.length;
                    },
                    [](std::vector<T, Alloc>& v) -> iterator {
                        return v.data() + v.size();
                    },
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
                overloaded{
                    [](const std::monostate&) -> size_t { return 0; },
                    [](const small_data& d) -> size_t { return d.length; },
                    [](const std::vector<T, Alloc>& v) -> size_t {
                        return v.size();
                    },
                },
                m_data);
        }

        size_type max_size() const
        {
            auto bytes = std::numeric_limits<size_t>::max();
            return bytes / sizeof(T);
        }

        void resize(size_t n)
        {
            switch (m_data.index()) {
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
            switch (m_data.index()) {
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

        const_reference at(size_t i) const
        {
            return std::visit(
                overloaded{
                    [&](const std::monostate&) -> const_reference {
                        throw std::out_of_range("out of range");
                    },
                    [&](const small_data& d) -> const_reference {
                        return d.data.at(i);
                    },
                    [&](const std::vector<T, Alloc>& v) -> const_reference {
                        return v.at(i);
                    },
                },
                m_data);
        }

        reference at(size_t i)
        {
            return std::visit(
                overloaded{
                    [&](std::monostate&) -> reference {
                        throw std::out_of_range("out of range");
                    },
                    [&](small_data& d) -> reference { return d.data.at(i); },
                    [&](std::vector<T, Alloc>& v) -> reference {
                        return v.at(i);
                    },
                },
                m_data);
        }

        reference back()
        {
            switch (m_data.index()) {
            default:
            case 0:
                MGE_THROW(illegal_state) << "back() called on empty vector";
                break;
            case 1:
                return std::get<1>(m_data).data[std::get<1>(m_data).length - 1];
                break;
            case 2:
                return std::get<2>(m_data).back();
                break;
            }
        }

        const_reference back() const
        {
            switch (m_data.index()) {
            default:
            case 0:
                MGE_THROW(illegal_state) << "back() called on empty vector";
                break;
            case 1:
                return std::get<1>(m_data).data[std::get<1>(m_data).length - 1];
                break;
            case 2:
                return std::get<2>(m_data).back();
                break;
            }
        }

        const_reference operator[](size_type i) const
        {
            return *(cbegin() + i);
        }

        reference operator[](size_type i)
        {
            return *(begin() + i);
        }

        size_type capacity() const
        {
            return std::visit(
                overloaded{
                    [](const std::monostate&) -> size_type { return 0; },
                    [](const small_data& d) -> size_type { return S; },
                    [](const std::vector<T, Alloc>& v) -> size_type {
                        return v.capacity();
                    },
                },
                m_data);
        }

        // emplace

        template <class... Args> reference emplace_back(Args&&... args)
        {
            return std::visit(
                overloaded{
                    [&](std::monostate&) -> reference {
                        m_data = small_data();
                        auto& sd = std::get<1>(m_data);
                        return sd.emplace_back(std::forward<Args>(args)...);
                    },
                    [&](small_data& d) -> reference {
                        if (d.length < S) {
                            return d.emplace_back(std::forward<Args>(args)...);
                        } else {
                            convert_to_vector();
                            return std::get<2>(m_data).emplace_back(
                                std::forward<Args>(args)...);
                        }
                    },
                    [&](std::vector<T, Alloc>& v) -> reference {
                        return v.emplace_back(std::forward<Args>(args)...);
                    },
                },
                m_data);
        }

        bool empty() const
        {
            return m_data.index() == 0;
        }

        pointer data() noexcept
        {
            return std::visit(
                overloaded{
                    [](std::monostate&) -> pointer { return nullptr; },
                    [](small_data& d) -> pointer { return d.data.data(); },
                    [](std::vector<T, Alloc>& v) -> pointer {
                        return v.data();
                    },
                },
                m_data);
        }

        const_pointer data() const noexcept
        {
            return std::visit(
                overloaded{
                    [](const std::monostate&) -> const_pointer {
                        return nullptr;
                    },
                    [](const small_data& d) -> const_pointer {
                        return d.data.data();
                    },
                    [](const std::vector<T, Alloc>& v) -> const_pointer {
                        return v.data();
                    },
                },
                m_data);
        }

        // erase

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *cbegin();
        }

        allocator_type get_allocator() const
        {
            return m_allocator;
        }

        template <typename I>
        constexpr iterator insert(const_iterator pos, I first, I last)
        {
            auto n = std::distance(first, last);
            if (n == 0) {
                return begin();
            } else if ((n < S) && (n + size() < S)) {
                return insert_small_data(pos, first, last, n);
            } else {
                return insert_vector(pos, first, last, n);
            }
        }

        void pop_back()
        {
            std::visit(
                overloaded{[](std::monostate&) {
                               throw std::out_of_range(
                                   "pop_back called on empty vector");
                           },
                           [](small_data& d) {
                               if (d.length == 0) {
                                   throw std::out_of_range(
                                       "pop_back called on empty vector");
                               }
                               d.data[--d.length].~T();
                           },
                           [](std::vector<T, Alloc>& v) {
                               if (v.empty()) {
                                   throw std::out_of_range(
                                       "pop_back called on empty vector");
                               }
                               v.pop_back();
                           }},
                m_data);
        }

    private:
        template <typename I>
        constexpr iterator
        insert_small_data(const_iterator pos, I first, I last, size_t n)
        {
            auto p = begin() + std::distance(cbegin(), pos);
            auto e = end();
            if (p == e) {
                for (; first != last; ++first) {
                    emplace_back(*first);
                }
            } else {
                auto i = end();
                for (; i != p; --i) {
                    *(i + n) = *i;
                }
                for (; first != last; ++first) {
                    *(i + n) = *first;
                }
                std::get<1>(m_data).length += n;
            }
            return p;
        }

        template <typename I>
        constexpr iterator
        insert_vector(const_iterator pos, I first, I last, size_t n)
        {
            convert_to_vector();
            auto& v = std::get<2>(m_data);
            auto  p = v.begin() + std::distance(cbegin(), pos);
            auto  r = v.insert(p, first, last);
            return v.data() + std::distance(v.begin(), r);
        }

    public:
        // pop_back

        void push_back(const value_type& val)
        {
            switch (m_data.index()) {
            case 0:
                m_data.template emplace<1>(1, val);
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
            switch (m_data.index()) {
            case 0:
                m_data.template emplace<1>(small_data());
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

        void reserve(size_type reserved_size)
        {
            if (m_data.index() == 2) {
                std::get<2>(m_data).reserve(reserved_size);
            }
        }

        void shrink_to_fit()
        {
            if (m_data.index() == 2) {
                std::get<2>(m_data).shrink_to_fit();
            }
        }

        void swap(small_vector<T, S, Alloc>& v)
        {
            m_data.swap(v.m_data);
        }

        void clear()
        {
            small_vector<T, S, Alloc> empty;
            swap(empty);
        }

        bool operator==(const small_vector<T, S, Alloc>& v) const
        {
            if (size() != v.size()) {
                return false;
            } else {
                return std::equal(begin(), end(), v.begin());
            }
        }

        bool operator!=(const small_vector<T, S, Alloc>& v) const
        {
            return !(*this == v);
        }

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
                std::get<1>(m_data).data[std::get<1>(m_data).length] =
                    std::move(val);
                std::get<1>(m_data).length++;
            }
        }

        void resize_from_monostate(size_t n)
        {
            if (n == 0) {
                return;
            } else if (n < S) {
                m_data.template emplace<1>(n, value_type());
            } else {
                m_data.template emplace<2>(n, value_type());
            }
        }

        void resize_from_monostate(size_t n, const value_type& fill)
        {
            if (n == 0) {
                return;
            } else if (n < S) {
                m_data.template emplace<1>(n, fill);
            } else {
                m_data.template emplace<2>(n, fill);
            }
        }

        void resize_from_small_data(size_t n)
        {
            if (n == 0) {
                m_data.template emplace<0>(std::monostate());
            } else if (n < std::get<1>(m_data).length) {
                for (size_t i = n; i < std::get<1>(m_data).length; ++i) {
                    std::get<1>(m_data).data[i] = value_type();
                }
                std::get<1>(m_data).length = n;
            } else if (n < S) {
                for (size_t i = std::get<1>(m_data).length; i < n; ++i) {
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
                m_data.template emplace<0>(std::monostate());
            } else if (n < std::get<1>(m_data).length) {
                for (size_t i = n; i < std::get<1>(m_data).length; ++i) {
                    std::get<1>(m_data).data[i] = fill;
                }
                std::get<1>(m_data).length = n;
            } else if (n < S) {
                for (size_t i = std::get<1>(m_data).length; i < n; ++i) {
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
            if (n == 0) {
                m_data.template emplace<0>(std::monostate());
            } else if (n > S) {
                std::get<2>(m_data).resize(n);
            } else {
                m_data.template emplace<1>(small_data(begin(), begin() + n));
            }
        }

        void resize_from_vector(size_t n, const value_type& fill)
        {
            if (n == 0) {
                m_data.template emplace<0>(std::monostate());
            } else if (n > S) {
                std::get<2>(m_data).resize(n, fill);
            } else {
                m_data.template emplace<1>(small_data(begin(), begin() + n));
            }
        }

        void convert_to_vector()
        {
            std::vector<value_type> v(begin(), end(), m_allocator);
            m_data.template emplace<2>(v);
        }

        using data_type =
            std::variant<std::monostate, small_data, std::vector<T, Alloc>>;

        data_type m_data;
        Alloc     m_allocator;
    };

} // namespace mge

namespace std {
    template <typename T, size_t S, class Alloc>
    struct hash<typename mge::small_vector<T, S, Alloc>>
    {
        std::size_t
        operator()(const mge::small_vector<T, S, Alloc>& v) const noexcept
        {
            std::size_t seed = 0;
            for (const auto& elem : v) {
                seed ^= std::hash<T>()(elem) + 0x9e3779b9 + (seed << 6) +
                        (seed >> 2);
            }
            return seed;
        }
    };
} // namespace std