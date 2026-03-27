unittest = require "unittest"

function test_mge_component_exists()
    assert(type(mge.register_component) == "function",
        "mge.register_component should be a function")
end

function test_component_arg1_must_be_table()
    local ok, err = pcall(mge.register_component, 42, {})
    assert(not ok, "should fail with non-table arg1")
    assert(string.find(err, "arg1"),
        "error should mention arg1")
end

function test_component_arg1_must_have_details()
    local ok, err = pcall(mge.register_component, {}, {})
    assert(not ok, "should fail with plain table arg1")
    assert(string.find(err, "arg1"),
        "error should mention arg1")
end

function test_component_arg1_must_have_proxy()
    -- mge.point is a class but has no proxy
    local ok, err = pcall(function()
        local Derived = class(mge.point)
        Derived.__name__ = "test_point_impl"
        mge.register_component(mge.point, Derived)
    end)
    assert(not ok, "should fail for type without proxy")
    assert(string.find(err, "proxy"),
        "error should mention proxy: " .. tostring(err))
end

function test_component_arg2_must_be_table()
    local ok, err = pcall(mge.register_component, mge.application, 42)
    assert(not ok, "should fail with non-table arg2")
    assert(string.find(err, "arg2"),
        "error should mention arg2")
end

function test_component_arg2_must_be_derived()
    local ok, err = pcall(function()
        local NotDerived = class()
        NotDerived.__name__ = "not_derived"
        mge.register_component(mge.application, NotDerived)
    end)
    assert(not ok, "should fail when arg2 not derived from arg1")
    assert(string.find(err, "arg2"),
        "error should mention arg2: " .. tostring(err))
end

function test_component_arg2_must_have_name()
    local ok, err = pcall(function()
        local MyApp = class(mge.application)
        mge.register_component(mge.application, MyApp)
    end)
    assert(not ok, "should fail without __name__")
    assert(string.find(err, "__name__"),
        "error should mention __name__: " .. tostring(err))
end

function test_component_registers_implementation()
    local MyApp = class(mge.application)
    MyApp.__name__ = "test_lua_app"
    mge.register_component(mge.application, MyApp)
    local impls = mge.registered_implementations("mge::application")
    local found = false
    for _, v in ipairs(impls) do
        if v == "test_lua_app" then
            found = true
            break
        end
    end
    assert(found,
        "test_lua_app should appear in registered implementations")
end

unittest.main()
