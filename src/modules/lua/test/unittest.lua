local unittest = {}

function unittest.fail()
    print("[  FAILED  ]")
    os.exit(1)
end

local function get_script_base()
    local src = arg and arg[0] or "unknown"
    src = src:gsub("\\", "/")
    local base = src:match("([^/]+)$") or src
    return base:gsub("%.lua$", "")
end

local function get_suite_name(base)
    local suite = base:match("^test%d+_(.+)$")
    return suite or base
end

local function clock_ms()
    return os.clock() * 1000
end

local function xml_escape(s)
    s = s:gsub("&", "&amp;")
    s = s:gsub("<", "&lt;")
    s = s:gsub(">", "&gt;")
    s = s:gsub("\"", "&quot;")
    s = s:gsub("'", "&apos;")
    return s
end

local function timestamp()
    return os.date("!%Y-%m-%dT%H:%M:%S")
end

local function write_junit_xml(filename, suite, results, suite_time_sec)
    local f = io.open(filename, "w")
    if not f then return end

    local num_tests = #results
    local num_failures = 0
    for _, r in ipairs(results) do
        if not r.success then num_failures = num_failures + 1 end
    end

    local ts = timestamp()
    f:write('<?xml version="1.0" encoding="UTF-8"?>\n')
    f:write(string.format(
        '<testsuites tests="%d" failures="%d" disabled="0" errors="0" time="%.3f" timestamp="%s" name="AllTests">\n',
        num_tests, num_failures, suite_time_sec, ts))
    f:write(string.format(
        '  <testsuite name="%s" tests="%d" failures="%d" disabled="0" skipped="0" errors="0" time="%.3f" timestamp="%s">\n',
        xml_escape(suite), num_tests, num_failures, suite_time_sec, ts))

    for _, r in ipairs(results) do
        local time_sec = r.elapsed_ms / 1000.0
        if r.success then
            f:write(string.format(
                '    <testcase name="%s" status="run" result="completed" time="%.3f" timestamp="%s" classname="%s" />\n',
                xml_escape(r.name), time_sec, ts, xml_escape(suite)))
        else
            f:write(string.format(
                '    <testcase name="%s" status="run" result="completed" time="%.3f" timestamp="%s" classname="%s">\n',
                xml_escape(r.name), time_sec, ts, xml_escape(suite)))
            f:write(string.format(
                '      <failure message="%s" type=""><![CDATA[%s]]></failure>\n',
                xml_escape(r.message), r.message))
            f:write('    </testcase>\n')
        end
    end

    f:write('  </testsuite>\n')
    f:write('</testsuites>\n')
    f:close()
end

function unittest.main()
    local base = get_script_base()
    local suite = get_suite_name(base)
    local tests = {}
    for key, item in pairs(_G) do
        local k = tostring(key)
        if k:find("test_", 1, true) == 1 and type(item) == "function" then
            local test_name = k:sub(6)
            if #test_name ~= 0 then
                table.insert(tests, {name = test_name, func = item})
            end
        end
    end

    table.sort(tests, function(a, b) return a.name < b.name end)

    local num_tests = #tests
    local suite_start = clock_ms()

    print(string.format("[==========] Running %d test%s from 1 test suite.",
        num_tests, num_tests == 1 and "" or "s"))
    print("[----------] Global test environment set-up.")
    print(string.format("[----------] %d test%s from %s",
        num_tests, num_tests == 1 and "" or "s", suite))

    local successful_tests, failed_tests = 0, 0
    local failed_names = {}
    local results = {}

    for _, item in ipairs(tests) do
        local full_name = suite .. "." .. item.name
        print(string.format("[ RUN      ] %s", full_name))
        local start = clock_ms()
        local success, details = pcall(item.func)
        local elapsed = math.floor(clock_ms() - start)
        if success then
            print(string.format("[       OK ] %s (%d ms)", full_name, elapsed))
            successful_tests = successful_tests + 1
            table.insert(results, {name = item.name, success = true, elapsed_ms = elapsed})
        else
            local msg = tostring(details)
            print(string.format("[  FAILED  ] %s (%d ms)", full_name, elapsed))
            print(msg)
            failed_tests = failed_tests + 1
            table.insert(failed_names, full_name)
            table.insert(results, {name = item.name, success = false, elapsed_ms = elapsed, message = msg})
        end
    end

    local suite_elapsed = math.floor(clock_ms() - suite_start)

    print(string.format("[----------] %d test%s from %s (%d ms total)",
        num_tests, num_tests == 1 and "" or "s", suite, suite_elapsed))
    print("")
    print("[----------] Global test environment tear-down")
    print(string.format("[==========] %d test%s from 1 test suite ran. (%d ms total)",
        num_tests, num_tests == 1 and "" or "s", suite_elapsed))
    print(string.format("[  PASSED  ] %d test%s.",
        successful_tests, successful_tests == 1 and "" or "s"))

    if failed_tests > 0 then
        print(string.format("[  FAILED  ] %d test%s, listed below:",
            failed_tests, failed_tests == 1 and "" or "s"))
        for _, name in ipairs(failed_names) do
            print(string.format("[  FAILED  ] %s", name))
        end
    end

    write_junit_xml("lua_" .. base .. ".xml", suite, results, suite_elapsed / 1000.0)

    if failed_tests > 0 then
        os.exit(1)
    else
        os.exit(0)
    end
end

return unittest