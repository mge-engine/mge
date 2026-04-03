-- blackscreen sample in Lua
-- Recreates the C++ blackscreen sample using the Lua scripting bindings.

local BlackScreen = class(mge.application)
BlackScreen.__name__ = "blackscreen_lua"

function BlackScreen:setup()
    self.render_system = mge.render_system.create()
    self.window = self.render_system:create_window()

    self.window:set_close_listener(function()
        self:set_quit()
    end)

    self.window:add_key_action_handler(function(k, action, modifier)
        if action == mge.key_action.PRESS and k == mge.key.ESCAPE then
            self:set_quit()
            return true
        end
        return false
    end)

    self:add_redraw_listener(function(cycle, delta)
        local ctx = self.window:render_context()
        local p = ctx:pass(0)
        p:default_viewport()
        p:clear_color(mge.rgba_color(0.0, 0.0, 0.0, 1.0))
        p:touch()
        ctx:frame()
    end)

    self.window:show()
end

function BlackScreen:teardown()
    self.window = nil
    self.render_system = nil
    collectgarbage("collect")
end

mge.register_component(mge.application, BlackScreen)
mge.application.main("blackscreen_lua")
