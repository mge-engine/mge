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

--[[
function test_mge_point_type_exists()
    local mge = package.loaded["mge"]
    local point_type = mge.point
    assert(type(point_type) == "table", "point type is not accessible")
end
--]]

unittest.main()