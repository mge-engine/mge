unittest = require "unittest"

function test_main_exists()
    assert(type(mge.application.main) == "function",
        "mge.application.main should be a function")
end

function test_main_runs_application()
    -- Define an application that quits immediately in setup
    local QuickApp = class(mge.application)
    QuickApp.__name__ = "test_quick_app"

    __quick_setup_called__ = false
    function QuickApp:setup()
        __quick_setup_called__ = true
        self:set_quit()
    end

    mge.register_component(mge.application, QuickApp)

    local rc = mge.application.main("test_quick_app")
    assert(__quick_setup_called__,
        "Lua setup() should have been called via main()")
    assert(rc == 0,
        "main() should return 0, got " .. tostring(rc))
end

unittest.main()
