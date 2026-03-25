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

function test_registered_implementations_exists()
    assert(type(mge.registered_implementations) == "function",
        "registered_implementations should be a function")
end

function test_registered_implementations_returns_table()
    local components = mge.registered_components()
    assert(#components > 0, "need at least one component")
    local result = mge.registered_implementations(components[1])
    assert(type(result) == "table",
        "registered_implementations should return a table")
end

function test_registered_implementations_contains_strings()
    local components = mge.registered_components()
    for _, comp in ipairs(components) do
        local impls = mge.registered_implementations(comp)
        for i, v in ipairs(impls) do
            assert(type(v) == "string",
                "each implementation should be a string, got " .. type(v))
        end
    end
end

unittest.main()
