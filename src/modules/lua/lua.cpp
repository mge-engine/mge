#include "mge/script/script_engine.hpp"

namespace lua {
    class lua_engine : public mge::script_engine
    {
    public:
        lua_engine() = default;
        virtual  ~lua_engine() = default;
    };

    MGE_REGISTER_IMPLEMENTATION(lua_engine, mge::script_engine, lua);
}
