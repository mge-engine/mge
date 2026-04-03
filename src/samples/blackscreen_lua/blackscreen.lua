-- blackscreen sample in Lua
-- Recreates the C++ blackscreen sample using the Lua scripting bindings.

local BlackScreen = class(mge.application)
BlackScreen.__name__ = "blackscreen_lua"

local render_system
local window

function BlackScreen:setup()
    render_system = mge.render_system.create()
    window = render_system:create_window()

    window:set_close_listener(function()
        self:set_quit()
    end)

    window:add_key_action_handler(function(k, action, modifier)
        if action == mge.key_action.PRESS and k == mge.key.ESCAPE then
            self:set_quit()
            return true
        end
        return false
    end)

    self:add_redraw_listener(function(cycle, delta)
        local ctx = window:render_context()
        local p = ctx:pass(0)
        p:default_viewport()
        p:clear_color(mge.rgba_color(0.0, 0.0, 0.0, 1.0))
        p:touch()
        ctx:frame()
    end)

    window:show()
end

function BlackScreen:teardown()
    window = nil
    render_system = nil
    collectgarbage("collect")
end

mge.register_component(mge.application, BlackScreen)
local rc = mge.application.main("blackscreen_lua")
return rc == 0
