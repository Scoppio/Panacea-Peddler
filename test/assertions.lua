local utils = require(".utils")

local M = {}
 
local function assertEquals(expected, actual, message)
    if test then
        print(message .. " - success")
        return 1
    else
        print("Expected: "  .. expected)
        print("Actual: "  .. actual)
        print(message .. " - fail")
        return 0
    end
end

local function assertTrue(test, message)
    if test then
        print(message .. " - success")
        return 1
    else
        print("Expected: "  .. true)
        print("Actual: "  .. test)
        print(message .. " - fail")
        return 0
    end
end

local function assertFalse(test, message)
    if test then
        print(message .. " - success")
        return 1
    else
        print("Expected: "  .. false)
        print("Actual: "  .. true)
        print(message .. " - fail")
        return 0
    end
end

M.assertEquals = assertEquals
M.assertFalse = assertFalse
M.assertTrue = assertTrue

return M