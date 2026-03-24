unittest = require "unittest"

function test_registered_components_exists()
    assert(type(mge.registered_components) == "function",
        "registered_components should be a function")
end

function test_registered_components_returns_table()
    local result = mge.registered_components()
    assert(type(result) == "table",
        "registered_components should return a table")
end

function test_registered_components_contains_strings()
    local result = mge.registered_components()
    assert(#result > 0, "registered_components should not be empty")
    for i, v in ipairs(result) do
        assert(type(v) == "string",
            "each element should be a string, got " .. type(v))
    end
end

unittest.main()
