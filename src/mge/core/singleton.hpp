// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <atomic>
#include <cstdlib>

namespace mge {
    /**
     * @brief Singleton instance of a class.
     *
     * @tparam T type of the singleton
     *
     * On demand initialization. Usually static variables
     * of @c singleton<T> are created for singleton objects
     * that should be lazily constructed.
     *
     * Note that the constructor of @c T must allow constructing
     * more than one object, as due to parallel access it may be
     * that more than one object is constructed, only when storing
     * the instance pointer the conflict is resolved and the superfluous
     * instance is destroyed.
     */
    template <typename T> class singleton
    {
    public:
        /**
         * @brief Value type.
         */
        using value_type = T;
        /**
         * @brief Pointer to value.
         */
        using pointer = T*;
        /**
         * @brief Pointer to constant value.
         */
        using const_pointer = const T*;
        /**
         * @brief Reference to value.
         */
        using reference = T&;
        /**
         * @brief Constant reference to value.
         */
        using const_reference = const T&;

        /**
         * Constructor. The contained object is not created.
         */
        singleton()
            : m_instance(nullptr)
        {}

        /**
         * Destructor. Exchanges the contained instance pointer
         * with the null pointer, and deletes the contained object.
         */
        ~singleton()
        {
            T* p = m_instance.load();
            T* null = nullptr;
            if (m_instance.compare_exchange_strong(p, null)) {
                delete p;
            } else {
                abort();
                //
                // crash("Inconsistency destroying singleton instance of",
                //      type_name<T>());
            }
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline pointer ptr()
        {
            T* p = m_instance.load();
            if (p) {
                return p;
            }

            p = new T();

            T* null = nullptr;

            if (m_instance.compare_exchange_strong(null, p)) {
                return p;
            } else {
                delete p;
                return ptr();
            }
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline pointer operator->()
        {
            return ptr();
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline const_pointer operator->() const
        {
            return ptr();
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline reference operator*()
        {
            return *ptr();
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline const_reference operator*() const
        {
            return *ptr();
        }

    private:
        std::atomic<T*> m_instance;
    };

} // namespace mge