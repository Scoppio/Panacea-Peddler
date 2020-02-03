local utils = require(".utils")

local M = {}


local function assertNotNull(actual, message)
    if actual then
        test_message(message)
        ctx.passed = ctx.passed + 1
    else
        test_message_on_fail(message)
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end


local function assertNotEquals(expected, actual, message)
    if equals(expected, actual) == false then
        test_message(message)
        ctx.passed = ctx.passed + 1
    else
        actual = actual and actual or "nil"
        expected = expected and expected or "nil"
        test_message_on_fail(message)
        print("Not Expected: ", expected)
        print("Actual: ", actual)
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function assertEquals(expected, actual, message)
    if equals(expected, actual) then
        test_message(message)
        ctx.passed = ctx.passed + 1
    else
        actual = actual and actual or "nil"
        expected = expected and expected or "nil"
        test_message_on_fail(message)
        print("Expected:", expected)
        print("Actual:", actual)
        print("")
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function assertTrue(test, message)
    if test then
        test_message(message)
        ctx.passed = ctx.passed + 1
    else
        actual = actual and actual or "nil"
        expected = expected and expected or "nil"
        test_message_on_fail(message)
        print("Expected: true")
        print("Actual: false")
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function assertFalse(test, message)
    if test == false then
        test_message(message)
        ctx.passed = ctx.passed + 1
    else
        actual = actual and actual or "nil"
        expected = expected and expected or "nil"
        test_message_on_fail(message)
        print("Expected: false")
        print("Actual: true")
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function getPassed()
    return ctx.passed
end

local function getTotal()
    return ctx.total_tests
end

local function getFailed()
    return ctx.failed
end

local function createTestContext(test_class)
    print("[".. test_class .. " test context]")
    ctx.test_class = test_class
    ctx.passed = 0
    ctx.failed = 0
    ctx.total_tests = 0
end

function test_message(message)
    print("[" .. ctx.total_tests+1 .. "] " .. ctx.test_class .. " - " .. message .. " - success")
end

function test_message_on_fail(message)
    print("[" .. ctx.total_tests+1 .. "] " .. ctx.test_class .. " - " .. message .. " - fail")
end

function equals(o1, o2, ignore_mt)
    if o1 == o2 then return true end
    local o1Type = type(o1)
    local o2Type = type(o2)
    if o1Type ~= o2Type then return false end
    if o1Type ~= 'table' then return false end

    if not ignore_mt then
        local mt1 = getmetatable(o1)
        if mt1 and mt1.__eq then
            --compare using built in method
            return o1 == o2
        end
    end

    local keySet = {}

    for key1, value1 in pairs(o1) do
        local value2 = o2[key1]
        if value2 == nil or equals(value1, value2, ignore_mt) == false then
            return false
        end
        keySet[key1] = true
    end

    for key2, _ in pairs(o2) do
        if not keySet[key2] then return false end
    end
    return true
end

ctx = {}

M.createTestContext = createTestContext
M.getTotal = getTotal
M.getFailed = getFailed
M.getPassed = getPassed

M.assertTrue = assertTrue
M.assertFalse = assertFalse
M.assertEquals = assertEquals
M.assertNotNull = assertNotNull
M.assertNotEquals = assertNotEquals

return M