local gint = require(".game_interface")
local assertions = require(".assertions")
local utils = require(".utils")

local M = {}
 
local function execute()
    -- up down left right start select A B
    assertions.createTestContext("deck_test")
     --  0 = blue
    --  1 = green
    --  2 = yellow
    --  3 = red
    local blue = gint.readCardOnCell(0).id ~= 0
    local green = gint.readCardOnCell(1).id ~= 16
    local yellow = gint.readCardOnCell(2).id ~= 32
    local red = gint.readCardOnCell(3).id ~= 48
    
    assertions.assertTrue(blue or green or yellow or red, "shuffle worked")
    
    return assertions.getPassed(), assertions.getTotal()
end

M.execute = execute
 
return M