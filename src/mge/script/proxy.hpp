// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    class invocation_context;

    /**
     * @brief Base class for proxy objects.
     *
     * Proxy objects need a virtual base to have only one invocation context.
     */
    class MGESCRIPT_EXPORT proxy_base
    {
    public:
        proxy_base();
        virtual ~proxy_base();

        /**
         * Set the invocation context.
         */
        void set_context(invocation_context* context);

    protected:
        invocation_context* m_context{nullptr};
    };

    /**
     * @brief Base class for proxy objects.
     *
     * A proxy object is a wrapper that allows implementing a derived class
     * in the scripting language, combining implementation in C++ and
     * scripting language.
     *
     * @tparam T base class of the proxy object
     */
    template <typename T> class proxy : public T, virtual public proxy_base
    {
    public:
        /**
         * @brief Base type of the proxy object.
         */
        using base_type = T;

        /**
         * @brief Default constructor.
         */
        proxy() = default;

        /**
         * @brief Constructor forwarding arguments to the base class.
         * @param args arguments to forward to the base class constructor
         */
        template <typename... Args>
        proxy(Args&&... args)
            : base_type(std::forward<Args>(args)...)
        {}

        /**
         * @brief Destructor.
         */
        virtual ~proxy() = default;

        /**
         * @brief Cast to proxy base.
         * @return proxy base
         */
        proxy_base* as_proxy_base()
        {
            return this;
        }
    };

} // namespace mge::script