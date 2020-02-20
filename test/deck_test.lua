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
    local blue = bit.rshift(gint.readCardOnCell(0).id, 4) == 0
    local green = bit.rshift(gint.readCardOnCell(1).id, 4) == 1
    local yellow = bit.rshift(gint.readCardOnCell(2).id, 4) == 2
    local red = bit.rshift(gint.readCardOnCell(3).id, 4) == 3
    
    assertions.assertTrue(blue and green and yellow and red, "shuffle worked")
    
    return assertions.getPassed(), assertions.getTotal()
end

M.execute = execute
 
return M