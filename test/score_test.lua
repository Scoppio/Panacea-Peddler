local gint = require(".game_interface")
local assertions = require(".assertions")
local utils = require(".utils")

local M = {}
 
local function execute()
    assertions.createTestContext("score_test")
    
    assertions.assertTrue(blue and green and yellow and red, "shuffle worked")
    
    return assertions.getPassed(), assertions.getTotal()
end

M.execute = execute
 
return M