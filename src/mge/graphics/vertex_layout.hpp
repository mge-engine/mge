#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/vertex_format.hpp"
#include <iosfwd>

namespace mge {

    /**
     * A vertex layout defines the layout of some vertex buffer, and such
     * is a list of vertex formats.
     */
    class MGEGRAPHICS_EXPORT vertex_layout : public mge::small_vector<vertex_format, 5>
    {
    public:
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
         * Move assigment.
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
    };

    MGEGRAPHICS_EXPORT std::ostream operator<<(std::ostream& os, const vertex_layout& l);

} // namespace mge
