// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/graphics/dllexport.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/point.hpp"
#include <iosfwd>

namespace mge {
    /**
     * A rectangle, that is, a position information and a width
     * and height.
     */
    class MGE_GRAPHICS_EXPORT rectangle
    {
    public:
       /**
        * Default constructor.
        */
       inline rectangle()
       :m_x(0),
        m_y(0),
        m_width(0),
        m_height(0)
       {}

       /**
        * Copy constructor.
        * @param d copied rectangle
        */
       rectangle(const rectangle& d)
       :m_x(d.m_x),
        m_y(d.m_y),
        m_width(d.m_width),
        m_height(d.m_height)
       {}

       /**
        * Construct from explicit values.
        * @param x      x position
        * @param y      y position
        * @param width  element width
        * @param height element height
        */
       rectangle(int x, int y, int width, int height)
       :m_x(x),
        m_y(y),
        m_width(width),
        m_height(height)
       {}

       /**
        * Construct using width and height. Position will be (0,0).
        * @param width element width
        * @param height element height
        */
       rectangle(int width, int height)
       :m_x(0),
        m_y(0),
        m_width(width),
        m_height(height)
       {}

       /**
        * Construct from initializer list, accepts either 2 values (only width
        * and height or 4 values (including position).
        * @param l initializer list of 2 or 4 values
        * @throws moge::illegal_argument if list size is not 2 or 4
        */
       rectangle(const std::initializer_list<int>& l)
       {
           if (l.size() == 2) {
               m_x = m_y = 0;
               std::initializer_list<int>::const_iterator i = l.begin();
               m_width = *i;
               ++i;
               m_height = *i;
           } else if (l.size() == 4) {
               std::initializer_list<int>::const_iterator i = l.begin();
               m_x = *i;
               ++i;
               m_y = *i;
               ++i;
               m_width = *i;
               ++i;
               m_height = *i;
               ++i;
           } else {
               MGE_THROW(illegal_argument)
                       << "Invalid size of initializer list: " << l.size();
           }
       }

       /**
        * Assignment.
        * @param d assigned rectangle
        * @return @c *this
        */
       inline rectangle& operator =(const rectangle& d)
       {
           m_x = d.m_x;
           m_y = d.m_y;
           m_width = d.m_width;
           m_height = d.m_height;
           return *this;
       }

       /**
        * Access width.
        * @return width
        */
       inline int width() const
       {
           return m_width;
       }

       /**
        * Access height.
        * @return height
        */
       inline int height() const
       {
           return m_height;
       }

       /**
        * Access x position.
        * @return x
        */
       inline int x() const
       {
           return m_x;
       }

       /**
        * Access y position.
        * @return y
        */
       inline int y() const
       {
           return m_y;
       }

       /**
        * Get left corner position.
        * @return left corner
        */
       inline int left() const
       {
           return m_x;
       }

       /**
        * Get right corner position.
        * @return right corner
        */
       inline int right() const
       {
           return m_x + m_width;
       }

       /**
        * Get top corner position.
        * @return top corner
        */
       inline int top() const
       {
           return m_y;
       }

       /**
        * Get bottom corner position.
        * @return bottom corner
        */
       inline int bottom() const
       {
           return m_y + m_height;
       }

        /**
          * Comparison operator.
          * @param d compared rectangle
          * @return @c true if equal
          */
        inline bool operator ==(const rectangle& d) const
        {
            return m_x == d.m_x && m_y == d.m_y && m_width == d.m_width
                    && m_height == d.m_height;
        }

        /**
         * Comparison operator.
         * @param d compared rectangle
         * @return @c true if not equal
         */
        inline bool operator !=(const rectangle& d) const
        {
            return !operator ==(d);
        }

       /**
        * Get rectangle extent.
        * @return extent
        */
        inline extent extent() const
        {
           return mge::extent(m_width, m_height);
        }

        /**
         * Get rectangle position.
         * 
         * @return upper left corner position
         */
        inline point position() const
        {
            return point(m_x, m_y);
        }

    private:
        uint32_t m_x;
        uint32_t m_y;
        uint32_t m_width;
        uint32_t m_height;
    };


    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const rectangle& r);

}
