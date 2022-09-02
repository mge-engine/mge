local unittest = {}

function unittest.fail()
    print("Tests failed")
    os.exit(1)
end

function unittest.assert(condition, ...)
    if(not condition) then
        local msg = ...
        if msg == nil then
            msg = "Assertion failed"
        end
        io.stderr:write(msg .. "\n")
        os.exit(1)
    end
end

return unittest