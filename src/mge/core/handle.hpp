// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/format.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"

namespace mge {

    /**
     * @brief Handle to an object managed by a specific context.
     * @tparam Object  type of the managed object
     * @tparam Context type of the context managing the object
     */
    template <typename Object, typename Context> class handle
    {
    public:
        /// @brief Context type.
        using context_type = Context;
        /// @brief Object type.
        using object_type = Object;

        /**
         * @brief Default constructor creates an invalid handle.
         */
        handle() = default;

        /**
         * @brief Construct a handle from context index, flags and object index.
         * @param context_index   index of the context managing the object
         * @param flags           flags for the object
         * @param object_index    index of the object in the context
         */
        handle(uint16_t context_index,
               uint16_t flags,
               uint32_t object_index) noexcept
            : m_context_index(context_index)
            , m_flags(flags)
            , m_object_index(object_index)
        {}

        /**
         * @brief Copy constructor.
         * @param other handle to copy
         */
        handle(const handle& other) noexcept
        {
            m_context_index = other.m_context_index;
            m_flags = other.m_flags;
            m_object_index = other.m_object_index;
        }

        handle(handle&& other) noexcept
        {
            m_context_index = other.m_context_index;
            m_flags = other.m_flags;
            m_object_index = other.m_object_index;
            other.m_context_index = 0xFFFF;
            other.m_flags = 0;
            other.m_object_index = 0xFFFFFFFF;
        }

        handle& operator=(const handle& other) noexcept
        {
            if (this != &other) {
                m_context_index = other.m_context_index;
                m_flags = other.m_flags;
                m_object_index = other.m_object_index;
            }
            return *this;
        }

        handle& operator=(handle&& other) noexcept
        {
            if (this != &other) {
                m_context_index = other.m_context_index;
                m_flags = other.m_flags;
                m_object_index = other.m_object_index;
                other.m_context_index = 0xFFFF;
                other.m_flags = 0;
                other.m_object_index = 0xFFFFFFFF;
            }
            return *this;
        }

        Object* get() const noexcept
        {
            Context* ctx = Context::get(m_context_index);
            if (!ctx) {
                return nullptr;
            }
            return ctx->template object<Object>(m_context_index,
                                                m_flags,
                                                m_object_index);
        }

        Object& operator*() const
        {
            auto* ptr = get();
            return *ptr;
        }

        Object* operator->() const noexcept
        {
            auto* ptr = get();
            return ptr;
        }

        operator bool() const noexcept
        {
            if (m_context_index == 0xFFFF || m_object_index == 0xFFFFFFFF) {
                return false;
            }
            return get() != nullptr;
        }

        void destroy()
        {
            Context* ctx = Context::get(m_context_index);
            if (!ctx) {
                MGE_THROW(illegal_state)
                    << "Destroying object from invalid context "
                    << mge::type_name<Context>();
            }
            ctx->template destroy_object<Object>(m_context_index,
                                                 m_flags,
                                                 m_object_index);
            m_context_index = 0xFFFF;
            m_flags = 0;
            m_object_index = 0xFFFFFFFF;
        }

    private:
        uint16_t m_context_index{0xFFFF};
        uint16_t m_flags{0};
        uint32_t m_object_index{0xFFFFFFFF};
    };

} // namespace mge

template <typename Object, typename Context>
struct fmt::formatter<mge::handle<Object, Context>>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::handle<Object, Context>& h, FormatContext& ctx) const
    {
        if (!h) {
            fmt::format_to(ctx.out(), "Invalid handle");
            return ctx.out();
        }
        fmt::format_to(ctx.out(), "handle<{}>", mge::type_name<Object>());
        return ctx.out();
    }
};