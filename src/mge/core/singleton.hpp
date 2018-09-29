// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"

#include <atomic>

namespace mge {
    /**
     * @brief Singleton instance of a class.
     *
     * On demand initialization. Usually @c static variables
     * of @c init_on_demand<T> are created for singleton objects
     * that should be lazily constructed.
     *
     * Note that the constructor of @c T must allow constructing
     * more than one object, as due to parallel access it may be
     * that more than one object is constructed, only when storing
     * the instance pointer the conflict is resolved and the superfluous
     * instance is destroyed.
     */
    template <typename T>
    class singleton : noncopyable
    {
    public:
        typedef T value_type;
        typedef T* pointer_type;
        typedef const T* const_pointer_type;
        typedef T& reference_type;
        typedef const T& const_reference_type;

        /**
         * Constructor. The contained object is not created.
         */
        singleton()
            :m_instance(nullptr)
        {}

        /**
         * Destructor. Exchanges the contained instance pointer
         * with the null pointer, and deletes the contained object.
         */
        ~singleton()
        {
            T * p = m_instance.load();
            T *null = nullptr;
            if(m_instance.compare_exchange_strong(p, null)) {
                delete p;
            } else {
                // TODO: CRASH
            }
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline pointer_type ptr()
        {
            T * p = m_instance.load();
            if(p) {
                return p;
            }
            p = new T();
            T * null = nullptr;
            if(m_instance.compare_exchange_strong(null, p)) {
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
        inline pointer_type operator ->()
        {
            return ptr();
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline const_pointer_type operator ->() const
        {
            return ptr();
        }

        /**
             * Access the contained object.
             * @return contained object
             */
        inline reference_type operator *()
        {
            return *ptr();
        }

        /**
         * Access the contained object.
         * @return contained object
         */
        inline const_reference_type operator *() const
        {
            return *ptr();
        }
    private:
        std::atomic<T*> m_instance;
    };
}
