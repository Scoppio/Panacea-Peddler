local gint = require(".game_interface")
local assertions = require(".assertions")

local M = {}
 
local function execute()
    
    print("Deck Shuffling Test")
    -- up down left right start select A B
    assertions.createTestContext("deck_test")
    assertions.assertEquals(gint.getGameState(), gamestates[2], "at correct game state")
    
    return 0, 0
end

M.execute = execute
 
return M