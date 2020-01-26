local utils = require(".utils")

local M = {}

local function execute(ram_table)
    local res = 0
    print("Menu Test::checking game state")
    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])
    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"

    print("GameState ".. gamestates[gs])

    if (gamestates[gs] == gamestates[0]) then
        print("Menu Test::Is on Menu - sucess")
        res = res +1
        local inputtable = {}
        inputtable["select"] = true
        joypad.write(1, inputtable)
        utils.wait(1)
        gs = memory.readbyte(ram_table["_GameState"])
        if (gamestates[gs] == gamestates[1]) then
            -- do nothing
            print("Menu Test::Enter on Menu Settings - sucess")
            res = res + 1
        else
            print("Menu Test::Enter on Menu Settings - fail")
        end
    else
        print("Menu Test::Is on Menu - fail")
        print("Menu Test::Enter on Menu Settings - fail")
    end
    return res, 2
end
M.execute = execute
 
return M