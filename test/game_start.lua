local utils = require(".utils")

local M = {}
 
local function execute(ram_table)
    local inputtable = {}
    
    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"

    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])

    print("Game on state", gamestates[gs])

    -- if not on game mode, move to game mode
    if (gamestates[gs] == gamestates[2]) 
    then
        return
    else
        inputtable["start"] = true
        joypad.write(1, inputtable)
        utils.wait(5, "Current mode = ".. gamestates[gs])
        execute(ram_table)
    end
end

local function goToMenu(ram_table)
    local inputtable = {}
    
    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"

    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])

    print("Game on state", gamestates[gs])

    -- if not on game mode, move to game mode
    if (gamestates[gs] == gamestates[2])
    then
        inputtable["start"] = true
        inputtable["select"] = true
        joypad.write(1, inputtable)
        utils.wait(5, "Current mode = ".. gamestates[gs])
        execute(ram_table)
    else 
        if (gamestates[gs] == gamestates[0])
        then
            --
        else
            inputtable["start"] = true
            joypad.write(1, inputtable)
            utils.wait(5, "Current mode = ".. gamestates[gs])
            execute(ram_table)
        end
    end
end

M.game = execute

M.menu = goToMenu

return M