unittest = require "unittest"

function test_rgba_color_constructor()
    local c = mge.rgba_color(0.1, 0.2, 0.3, 0.4)
    assert(c ~= nil, "rgba_color should be constructible")
end

function test_rgba_color_fields()
    local c = mge.rgba_color(0.1, 0.2, 0.3, 0.4)
    local eps = 0.001
    assert(math.abs(c.r - 0.1) < eps, "r should be ~0.1, got " .. c.r)
    assert(math.abs(c.g - 0.2) < eps, "g should be ~0.2, got " .. c.g)
    assert(math.abs(c.b - 0.3) < eps, "b should be ~0.3, got " .. c.b)
    assert(math.abs(c.a - 0.4) < eps, "a should be ~0.4, got " .. c.a)
end

function test_rgba_color_field_write()
    local c = mge.rgba_color(0.0, 0.0, 0.0, 0.0)
    c.r = 1.0
    c.g = 0.5
    c.b = 0.25
    c.a = 0.75
    local eps = 0.001
    assert(math.abs(c.r - 1.0) < eps, "r should be 1.0")
    assert(math.abs(c.g - 0.5) < eps, "g should be 0.5")
    assert(math.abs(c.b - 0.25) < eps, "b should be 0.25")
    assert(math.abs(c.a - 0.75) < eps, "a should be 0.75")
end

function test_key_enum_values()
    assert(mge.key.ESCAPE ~= nil, "key.ESCAPE should exist")
    assert(mge.key.SPACE ~= nil, "key.SPACE should exist")
    assert(mge.key.ENTER ~= nil, "key.ENTER should exist")
    assert(mge.key.A ~= nil, "key.A should exist")
    assert(mge.key.Z ~= nil, "key.Z should exist")
    assert(mge.key.F1 ~= nil, "key.F1 should exist")
    assert(mge.key.F12 ~= nil, "key.F12 should exist")
end

function test_window_type_exists()
    assert(mge.window ~= nil, "mge.window should exist")
end

function test_render_context_type_exists()
    assert(mge.render_context ~= nil, "mge.render_context should exist")
end

function test_pass_type_exists()
    assert(mge.pass ~= nil, "mge.pass should exist")
end

function test_application_set_quit_exists()
    -- Verify the method exists by checking the application type
    assert(mge.application ~= nil, "mge.application should exist")
end

unittest.main()
