// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
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
                return (&m_layout == &i.m_layout) && (m_index == i.m_index);
            }

            bool operator!=(const iterator& i) const
            {
                return (&m_layout != &i.m_layout) || m_index != i.m_index;
            }

            entry operator*()
            {
                entry e{m_layout.m_formats[m_index],
                        m_layout.m_semantics[m_index]};
                return e;
            }

            entry operator->()
            {
                entry e{m_layout.m_formats[m_index],
                        m_layout.m_semantics[m_index]};
                return e;
            }

        private:
            vertex_layout& m_layout;
            size_t         m_index;
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
                return (&m_layout == &i.m_layout) && (m_index == i.m_index);
            }

            bool operator!=(const const_iterator& i) const
            {
                return (&m_layout != &i.m_layout) || m_index != i.m_index;
            }

            const_entry operator*() const
            {
                const_entry e{m_layout.m_formats[m_index],
                              m_layout.m_semantics[m_index]};
                return e;
            }

            const_entry operator->()
            {
                const_entry e{m_layout.m_formats[m_index],
                              m_layout.m_semantics[m_index]};
                return e;
            }

        private:
            const vertex_layout& m_layout;
            size_t               m_index;
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
         * @brief Append a format.
         *
         * @param f format
         */
        void push_back(const vertex_format& f);

        /**
         * @brief Formats of vertex layout.
         *
         * @return formats
         */
        const auto& formats() const noexcept { return m_formats; }

        /**
         * @brief Semantics of vertex layout.
         *
         * @return semantics
         */
        const auto& semantics() const noexcept { return m_semantics; }

        iterator       begin() { return iterator(*this, 0); }
        iterator       end() { return iterator(*this, m_formats.size()); }
        const_iterator begin() const { return const_iterator(*this, 0); }
        const_iterator end() const
        {
            return const_iterator(*this, m_formats.size());
        }
        const_iterator cbegin() const { return const_iterator(*this, 0); }
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

        inline entry at(size_t index)
        {
            return entry{m_formats.at(index), m_semantics.at(index)};
        }

        inline const_entry at(size_t index) const
        {
            return const_entry{m_formats.at(index), m_semantics.at(index)};
        }

        auto size() const noexcept { return m_formats.size(); }

    private:
        mge::small_vector<vertex_format, 3>      m_formats;
        mge::small_vector<attribute_semantic, 3> m_semantics;

        friend MGEGRAPHICS_EXPORT std::ostream&
        operator<<(std::ostream& os, const vertex_layout& l);
    };

    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&        os,
                                                const vertex_layout& l);

    MGEGRAPHICS_EXPORT mge::vertex_layout
                       parse_vertex_layout(std::string_view sv);

} // namespace mge
