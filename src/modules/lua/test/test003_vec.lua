unittest = require "unittest"

function test_fvec2_type_exists()
    local mge = package.loaded["mge"]
    local fvec2_type = mge.fvec2
    assert(type(fvec2_type) == "table", "fvec2 type is not accessible")
end

unittest.main()