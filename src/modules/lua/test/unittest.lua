local unittest = {}

function unittest.fail()
    print("Tests failed")
    os.exit(1)
end


function unittest.main()
    tests = {}
    max_test_name_len = 0
    for key, item in pairs(_G)
    do
        k = tostring(key)
        if k:find("test_", 1, true) == 1 and type(item) == "function"
        then
            local test_name = k:sub(6)
            if #test_name ~= 0
            then
                if #test_name > max_test_name_len
                then
                    max_test_name_len = #test_name
                end
                table.insert(tests, {name=test_name, func = item})
            end
        end
    end
    if #tests == 1
    then
        print("Running 1 test")
    else
        print(string.format("Running %d tests", #tests))
    end
    --   <test> .. status

    spaces = string.rep(" ", max_test_name_len)
    successful_tests, failed_tests = 0,0
    for _, item in pairs(tests)
    do
        n = item.name
        f = item.func
        padding = spaces:sub(#n+1)
        io.stdout:write(string.format("  %s%s .. ", n, padding))
        success, details = pcall(f)
        if success
        then
            io.stdout:write("OK")
            successful_tests = successful_tests + 1
        else
            io.stdout:write(string.format("FAILED %s", tostring(details)))
            failed_tests = failed_tests + 1
        end
        io.stdout:write("\n")
        io.stdout:flush()
    end

    print(string.format("%d tests executed, %d ok, %d failed",
        successful_tests+failed_tests,
        successful_tests,
        failed_tests))
    if failed_tests > 0
    then
        os.exit(1)
    else
        os.exit(0)
    end
end

return unittest