local utils = require(".utils")
local assertions = require(".assertions")
local gint = require(".game_interface")

local M = {}

local function execute()
    assertions.createTestContext("menu_test")
    
    gint.input("select")
    local gs = gint.getGameState()
    assertions.assertEquals("MENU_SETTINGS", gs, "Enter Menu Settings from Menu")
    return assertions.getPassed(), assertions.getTotal()
end

M.execute = execute
 
return M