local utils = require(".utils")

local M = {}
 
local function assertEquals(expected, actual, message)
    if expected == actual then
        print(ctx.test_class .. " - " .. message .. " - success")
        ctx.passed = ctx.passed + 1
    else
        print("Expected: "  .. expected)
        print("Actual: "  .. actual)
        print(ctx.test_class .. " - " .. message .. " - fail")
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function assertTrue(test, message)
    if test then
        print(ctx.test_class .. " - " .. message .. " - success")
        ctx.passed = ctx.passed + 1
    else
        print("Expected: "  .. true)
        print("Actual: "  .. test)
        print(ctx.test_class .. " - " .. message .. " - fail")
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function assertFalse(test, message)
    if test == false then
        print(ctx.test_class .. " - " .. message .. " - success")
        ctx.passed = ctx.passed + 1
    else
        print("Expected: "  .. false)
        print("Actual: "  .. true)
        print(ctx.test_class .. " - " .. message .. " - fail")
        ctx.failed = ctx.failed + 1
    end
    ctx.total_tests = ctx.total_tests + 1
end

local function getPassed()
    return ctx.passed
end

local function getTotal()
    return ctx.passed
end

local function getFailed()
    return ctx.passed
end

local function createTestContext(test_class)
    ctx.test_class = test_class and test_class or "assertions"
    ctx.passed = 0
    ctx.failed = 0
    ctx.total_tests = 0    
end

ctx = {}

M.getTotal = getTotal
M.getPassed = getPassed
M.getFailed = getFailed
M.createTestContext = createTestContext
M.assertEquals = assertEquals
M.assertFalse = assertFalse
M.assertTrue = assertTrue
M.getTestResults = getTestResults

return M