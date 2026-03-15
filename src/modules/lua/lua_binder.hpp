// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/reflection/type_details.hpp"
#include "mge/reflection/visitor.hpp"
#include "mge/script/script_context.hpp"

#include <memory>

namespace mge::lua {

    class lua_context;

    enum class lua_instance_ownership : uint8_t
    {
        OWNED,
        FOREIGN_POINTER,
        SHARED_PTR
    };

    /**
     * @brief Header embedded in full userdata for class instances.
     *
     * The object data follows immediately after this header in memory.
     * When ownership is @c FOREIGN_POINTER, the data area stores a void*
     * pointing to externally-owned memory (no destruction on GC).
     * When ownership is @c SHARED_PTR, the data area stores a
     * std::shared_ptr<void> keeping the object alive.
     */
    struct lua_instance_header
    {
        const mge::reflection::type_details* type;
        lua_instance_ownership               ownership;
    };

    class lua_binder : public mge::reflection::visitor
    {
    public:
        lua_binder(lua_context* context)
            : m_context(context)
        {}

        ~lua_binder() = default;

        void before(const mge::reflection::module_details& details) override;
        void on(const mge::reflection::module_details& details) override;
        void after(const mge::reflection::module_details& details) override;

        void before(const mge::reflection::type_details& details) override;
        void on(const mge::reflection::type_details& details) override;
        void after(const mge::reflection::type_details& details) override;

        void before(const mge::reflection::function_details& details) override;
        void on(const mge::reflection::function_details& details) override;
        void after(const mge::reflection::function_details& details) override;

        static void* instance_object_ptr(lua_instance_header* header);

    private:
        void
        bind_enum(std::string_view name,
                  const mge::reflection::type_details::enum_specific_details&
                      details);
        void bind_class(const mge::reflection::type_details& details);
        void
        create_instance_metatable(const mge::reflection::type_details& details);

        static int class_call(lua_State* L);
        static int instance_index(lua_State* L);
        static int instance_newindex(lua_State* L);
        static int instance_gc(lua_State* L);
        static int method_call(lua_State* L);
        static int static_method_call(lua_State* L);

    public:
        static void create_foreign_instance(
            lua_State*                           L,
            const mge::reflection::type_details* target_type,
            void*                                ptr);

        static void create_shared_instance(
            lua_State*                           L,
            const mge::reflection::type_details* element_type,
            std::shared_ptr<void>                ptr);

    private:
        lua_context* m_context;
    };

} // namespace mge::lua