// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/small_vector.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/attribute_semantic.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/vertex_format.hpp"
#include <iosfwd>
#include <string_view>

namespace mge {

    /**
     * A vertex layout defines the layout of some vertex buffer, and such
     * is a list of vertex formats.
     */
    class MGEGRAPHICS_EXPORT vertex_layout
    {
    public:
        /**
         * @brief Entry in a vertex layout.
         */
        struct entry
        {
            vertex_format&      format;
            attribute_semantic& semantic;
        };

        /**
         * @brief Const entry in a vertex layout.
         */
        struct const_entry
        {
            const vertex_format&      format;
            const attribute_semantic& semantic;
        };

        class iterator
        {
        public:
            iterator(vertex_layout& l, size_t index)
                : m_layout(l)
                , m_index(index)
            {}

            iterator(const iterator& i) = default;
            iterator(iterator&& i) = default;

            iterator& operator=(const iterator& i) = default;
            iterator& operator=(iterator&& i) = default;

            iterator& operator++()
            {
                ++m_index;
                return *this;
            }

            iterator operator++(int)
            {
                iterator tmp(*this);
                ++m_index;
                return tmp;
            }

            bool operator==(const iterator& i) const
            {
                return (&m_layout.get() == &i.m_layout.get()) &&
                       (m_index == i.m_index);
            }

            bool operator!=(const iterator& i) const
            {
                return (&m_layout.get() != &i.m_layout.get()) ||
                       m_index != i.m_index;
            }

            entry operator*()
            {
                entry e{m_layout.get().m_formats[m_index],
                        m_layout.get().m_semantics[m_index]};
                return e;
            }

            entry operator->()
            {
                entry e{m_layout.get().m_formats[m_index],
                        m_layout.get().m_semantics[m_index]};
                return e;
            }

        private:
            std::reference_wrapper<vertex_layout> m_layout;
            size_t                                m_index;
        };

        class const_iterator
        {
        public:
            const_iterator(const vertex_layout& l, size_t index)
                : m_layout(l)
                , m_index(index)
            {}

            const_iterator(const const_iterator& i) = default;
            const_iterator(const_iterator&& i) = default;

            const_iterator& operator=(const const_iterator& i) = default;
            const_iterator& operator=(const_iterator&& i) = default;

            const_iterator& operator++()
            {
                ++m_index;
                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator tmp(*this);
                ++m_index;
                return tmp;
            }

            bool operator==(const const_iterator& i) const
            {
                return (&m_layout.get() == &i.m_layout.get()) &&
                       (m_index == i.m_index);
            }

            bool operator!=(const const_iterator& i) const
            {
                return (&m_layout.get() != &i.m_layout.get()) ||
                       m_index != i.m_index;
            }

            const_entry operator*() const
            {
                const_entry e{m_layout.get().m_formats[m_index],
                              m_layout.get().m_semantics[m_index]};
                return e;
            }

            const_entry operator->() const
            {
                const_entry e{m_layout.get().m_formats[m_index],
                              m_layout.get().m_semantics[m_index]};
                return e;
            }

        private:
            std::reference_wrapper<const vertex_layout> m_layout;
            size_t                                      m_index;
        };

        /**
         * Constructor.
         */
        vertex_layout();

        /**
         * @brief Constructor.
         *
         * @param l initializers
         */
        vertex_layout(const std::initializer_list<vertex_format>& l);

        /**
         * Copy constructor.
         *
         * @param l copied layout
         */
        vertex_layout(const vertex_layout& l) = default;
        /**
         * Move constructor.
         *
         * @param l moved layout
         */
        vertex_layout(vertex_layout&& l) = default;
        /**
         * Destructor.
         */
        ~vertex_layout() = default;
        /**
         * Assignment.
         *
         * @param l assigned layout
         * @return @c *this
         */
        vertex_layout& operator=(const vertex_layout& l) = default;
        /**
         * Move assignment.
         *
         * @param l moved layout
         * @return @c *this
         */
        vertex_layout& operator=(vertex_layout&& l) = default;

        /**
         * Get the binary size of one element as defined by the layout.
         *
         * @return bytes needed for one element
         */
        size_t binary_size() const;

        /**
         * Get the stride (binary size) of the layout.
         *
         * @return stride
         */
        inline size_t stride() const
        {
            return binary_size();
        }

        /**
         * Get the offset of a specific index in the layout.
         *
         * @param index index of format
         * @return offset in bytes
         */
        size_t offset(size_t index) const;

        /**
         * @brief Append a format.
         *
         * @param f format
         */
        void push_back(const vertex_format& f);

        /**
         * @brief Append a format with a semantic.
         *
         * @param f format
         * @param s semantic
         */
        void push_back(const vertex_format& f, attribute_semantic s);

        /**
         * @brief Formats of vertex layout.
         *
         * @return formats
         */
        const auto& formats() const noexcept
        {
            return m_formats;
        }

        /**
         * @brief Semantics of vertex layout.
         *
         * @return semantics
         */
        const auto& semantics() const noexcept
        {
            return m_semantics;
        }

        /**
         * @brief Iterator to the first element.
         *
         * @return begin iterator
         */
        iterator begin()
        {
            return iterator(*this, 0);
        }

        /**
         * @brief Iterator past the last element.
         *
         * @return end iterator
         */
        iterator end()
        {
            return iterator(*this, m_formats.size());
        }

        /**
         * @brief Const iterator to the first element.
         *
         * @return begin const iterator
         */
        const_iterator begin() const
        {
            return const_iterator(*this, 0);
        }

        /**
         * @brief Const iterator past the last element.
         *
         * @return end const iterator
         */
        const_iterator end() const
        {
            return const_iterator(*this, m_formats.size());
        }

        /**
         * @brief Const iterator to the first element.
         *
         * @return begin const iterator
         */
        const_iterator cbegin() const
        {
            return const_iterator(*this, 0);
        }

        /**
         * @brief Const iterator past the last element.
         *
         * @return end const iterator
         */
        const_iterator cend() const
        {
            return const_iterator(*this, m_formats.size());
        }

        inline entry operator[](size_t index)
        {
            return entry{m_formats[index], m_semantics[index]};
        }

        inline const_entry operator[](size_t index) const
        {
            return const_entry{m_formats[index], m_semantics[index]};
        }

        /**
         * @brief Access element at index with bounds checking.
         *
         * @param index element index
         * @return entry at index
         */
        inline entry at(size_t index)
        {
            return entry{m_formats.at(index), m_semantics.at(index)};
        }

        /**
         * @brief Access element at index with bounds checking (const).
         *
         * @param index element index
         * @return const entry at index
         */
        inline const_entry at(size_t index) const
        {
            return const_entry{m_formats.at(index), m_semantics.at(index)};
        }

        /**
         * @brief Number of elements in the layout.
         *
         * @return element count
         */
        auto size() const noexcept
        {
            return m_formats.size();
        }

        inline bool operator==(const vertex_layout& l) const noexcept
        {
            return m_formats == l.m_formats && m_semantics == l.m_semantics;
        }

        inline bool operator!=(const vertex_layout& l) const noexcept
        {
            return !(*this == l);
        }

    private:
        mge::small_vector<vertex_format, 3>      m_formats;
        mge::small_vector<attribute_semantic, 3> m_semantics;
    };

    MGEGRAPHICS_EXPORT mge::vertex_layout
                       parse_vertex_layout(std::string_view sv);

} // namespace mge

template <> struct std::hash<mge::vertex_layout>
{
    size_t operator()(const mge::vertex_layout& l) const noexcept
    {
        size_t h = 0;
        for (size_t i = 0; i < l.size(); ++i) {
            auto entry = l[i];
            h = std::hash<mge::vertex_format>{}(entry.format);
            h = h * 31 + std::hash<mge::attribute_semantic>{}(entry.semantic);
        }
        return h;
    }
};

template <>
struct fmt::formatter<mge::vertex_layout>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::vertex_layout& l, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "[");
        mge::vertex_layout::const_iterator it = l.cbegin();
        while (it != l.cend()) {
            const auto entry = *it;
            fmt::format_to(ctx.out(),
                           "{{format={}, semantic={}}}",
                           entry.format,
                           entry.semantic);
            ++it;
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const vertex_layout& l)
    {
        fmt::print(os, "{}", l);
        return os;
    }
} // namespace mge
