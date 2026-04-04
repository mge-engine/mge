unittest = require "unittest"

function test_key_action_enum_exists()
    assert(mge.key_action ~= nil, "key_action should exist")
    assert(mge.key_action.PRESS ~= nil, "key_action.PRESS should exist")
    assert(mge.key_action.RELEASE ~= nil, "key_action.RELEASE should exist")
    assert(mge.key_action.REPEAT ~= nil, "key_action.REPEAT should exist")
end

function test_add_redraw_listener()
    local MyApp = class(mge.application)
    MyApp.__name__ = "test_callback_app"

    __listener_registered__ = false
    function MyApp:setup()
        self:add_redraw_listener(function(cycle, delta)
            -- not called in this test (no event loop)
        end)
        __listener_registered__ = true
    end

    mge.register_component(mge.application, MyApp)
    local app = mge.create_component(mge.application,
                                     "test_callback_app")
    app:setup()
    assert(__listener_registered__,
        "redraw listener should have been registered")
    app = nil
    collectgarbage("collect")
end

unittest.main()
