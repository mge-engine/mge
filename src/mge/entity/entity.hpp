#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity_fwd.hpp"

#include <cstdint>
#include <iosfwd>

namespace mge {
    
    /**
     * An entity. An entity is represented by its id, which is stable
     * over its lifetime.
     */
    class MGE_ENTITY_EXPORT entity 
    {
    public:
        /**
         * Type of entity id.
         */
        typedef uint64_t id_type;

        /**
         * Constructs nil entity.
         */
        entity() noexcept
            :m_id(0)
        {}

        /**
         * Shortcut to nil/null entity.
         * @return the nil entity
         */
        static inline entity nil() noexcept { return entity(); }

        /**
         * Shortcut to nil/null entity.
         * @return the nil entity
         */
        static inline entity null() noexcept { return entity(); }
    private:
        friend class entity_registry;

        explicit entity(uint64_t id)
            :m_id(id)
        {}
    public:
        /**
         * Copy constructor
         * @param e copied entity
         */
        entity(const entity& e) = default;

        /**
         * Move constructor.
         * @param e moved entity
         */
        entity(entity&& e) = default;

        /**
         * Assignment operator.
         * @param e assigned entity
         * @return @c *this
         */
        entity& operator =(const entity& e) = default;

        /**
         * Move assignment.
         * @param e assigned entity
         * @return @c *this
         */
        entity& operator =(entity&& e) = default;

        /**
         * Retrieve id.
         * 
         * @return entity id
         */
        inline id_type id() const noexcept { return m_id; }
        inline bool is_null() const noexcept { return m_id == 0; }

        inline bool operator ==(const entity& other) const noexcept
        {
            return m_id == other.m_id;
        }

        inline bool operator !=(const entity& other) const noexcept
        {
            return m_id != other.m_id;
        }

        inline bool operator <(const entity& other) const noexcept
        {
            return m_id < other.m_id;
        }
    private:
        uint64_t m_id;
    };
}