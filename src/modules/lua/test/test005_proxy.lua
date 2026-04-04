unittest = require "unittest"

function test_application_construct()
    local app = mge.application()
    assert(type(app) == "userdata", "application not created as userdata")
    app = nil
    collectgarbage()
end

unittest.main()
