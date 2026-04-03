unittest = require "unittest"

function test_create_component_exists()
    assert(type(mge.create_component) == "function",
        "mge.create_component should be a function")
end

function test_lua_setup_override_fires()
    -- Define a Lua class derived from application
    local MyApp = class(mge.application)
    MyApp.__name__ = "test_proxy_dispatch_app"

    -- Track whether setup() was called
    __setup_called__ = false
    function MyApp:setup()
        __setup_called__ = true
    end

    mge.register_component(mge.application, MyApp)

    -- Create instance via component factory
    local app = mge.create_component(mge.application,
                                     "test_proxy_dispatch_app")
    assert(app ~= nil, "create_component should return an instance")

    -- Call setup() which should dispatch to the Lua override
    app:setup()
    assert(__setup_called__,
        "Lua setup() override should have been called")

    -- application is a singleton; release before next test
    app = nil
    collectgarbage("collect")
end

function test_lua_teardown_override_fires()
    local MyApp2 = class(mge.application)
    MyApp2.__name__ = "test_proxy_dispatch_app2"

    __teardown_called__ = false
    function MyApp2:teardown()
        __teardown_called__ = true
    end

    mge.register_component(mge.application, MyApp2)

    local app = mge.create_component(mge.application,
                                     "test_proxy_dispatch_app2")
    app:teardown()
    assert(__teardown_called__,
        "Lua teardown() override should have been called")

    -- application is a singleton; release before next test
    app = nil
    collectgarbage("collect")
end

function test_lua_override_not_found_calls_base()
    -- A class that does NOT override setup()
    local MyApp3 = class(mge.application)
    MyApp3.__name__ = "test_proxy_dispatch_app3"

    mge.register_component(mge.application, MyApp3)

    local app = mge.create_component(mge.application,
                                     "test_proxy_dispatch_app3")
    -- Should call base application::setup() without error
    app:setup()

    app = nil
    collectgarbage("collect")
end

unittest.main()
