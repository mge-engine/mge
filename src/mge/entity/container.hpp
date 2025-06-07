// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entity.hpp"

namespace mge::entity {

    class MGEENTITY_EXPORT container : public entity
    {
    public:
        explicit container(mge::entity::registry& w);
        container(mge::entity::registry& w, const char* name);
        explicit container(const mge::entity::entity& e) noexcept;

        container(const container&) = default;
        container& operator=(const container&) = default;
        container(container&&) noexcept = default;
        container& operator=(container&&) noexcept = default;
        ~container();

        template <has_entity T> void insert(const T& object)
        {
            add(object.entity());
        }

#if 0
        // Iterators
        using iterator = std::unordered_set<mge::entity::entity>::iterator;
        using const_iterator = std::unordered_set<mge::entity::entity>::const_iterator;

        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        // Capacity
        bool empty() const noexcept;
        size_t size() const noexcept;
        size_t max_size() const noexcept;

        // Modifiers
        void clear() noexcept;
        std::pair<iterator, bool> insert(const mge::entity::entity& entity);
        std::pair<iterator, bool> insert(mge::entity::entity&& entity);
        template <class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);
        iterator erase(const_iterator position);
        size_t erase(const mge::entity::entity& entity);
        iterator erase(const_iterator first, const_iterator last);
        void swap(container& other) noexcept;

        // Lookup
        iterator find(const mge::entity::entity& entity);
        const_iterator find(const mge::entity::entity& entity) const;
        size_t count(const mge::entity::entity& entity) const;
        bool contains(const mge::entity::entity& entity) const;

        // Hash policy
        float load_factor() const noexcept;
        float max_load_factor() const noexcept;
        void max_load_factor(float ml);
        void rehash(size_t count);
        void reserve(size_t count);
#endif
    private:
        void add(const mge::entity::entity& entity);
    };

} // namespace mge::entity