unittest = require "unittest"

function test_mge_module_exists()
    local mod = package.loaded["mge"]
    assert(type(mod) == "table", "mge module seems not be loaded")
    assert(type(mge) == "table", "mge module not accessible by global var")
end

function test_mge_data_type_exists()
    assert(type(mge.data_type) == "table")
    assert(mge.data_type.HALF == 11)
end

function test_mge_point_type_exists()
    local mge = package.loaded["mge"]
    local point_type = mge.point
    assert(type(point_type) == "table", "point type is not accessible")
end

function test_mge_point_construct()
    local mge = package.loaded["mge"]
    p = mge.point:new(123, 456)
    assert(type(p) == "userdata", "point not created as wanted")
    p = nil
    collectgarbage()
end

function test_application_similar_subtype_exists()
    local mge = package.loaded["mge"]
    local il_type = mge.application.input_listener
    local ql_type = mge.application.quit_listener
    assert(type(il_type) == "table", "input_listener type is not accessible")
    assert(type(ql_type) == "table", "quit_listener type is not accessible")
end

function test_method_call()
    local vf = mge.vertex_format:new(mge.data_type.FLOAT, 3)
    assert(type(vf) == "userdata", "cannot create object")
    assert(vf.binary_size() == 12, "cannot call simple method")
end

unittest.main()