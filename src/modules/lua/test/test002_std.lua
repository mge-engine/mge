unittest = require "unittest"

function test_std_module_exists()
    local mod = package.loaded["std"]
    assert(type(mod) == "table", "std module seems not be loaded")
end

unittest.main()