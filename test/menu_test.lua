local utils = require(".utils")
local assertions = require(".assertions")
local gint = require(".game_interface")

local M = {}

local function execute()
    -- up down left right start select A B
    assertions.createTestContext("menu_test")
    gint.input("select")
    assertions.assertEquals(gamestates[1], gint.getGameState(), "Enter Menu Settings from Menu")
    return ctx.passed, ctx.total_tests
end

M.execute = execute
 
return M