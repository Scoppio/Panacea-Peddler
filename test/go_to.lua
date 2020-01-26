local utils = require(".utils")

local M = {}
 
local function game(ram_table)
    local inputtable = {}
    
    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"

    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])

    print("Game on state", gamestates[gs], "target is ".. gamestates[2])

    -- if not on game mode, move to game mode
    if (gamestates[gs] == gamestates[2]) 
    then
        return
    else
        inputtable["start"] = true
        joypad.write(1, inputtable)
        utils.wait(5, "Current mode = ".. gamestates[gs])
        game(ram_table)
    end
end

local function menu(ram_table)
    
    local function moveAndUpdate(key)
        local tt = {}
        tt[key] = true
        joypad.write(1, tt)
        utils.wait(5, "Changing gameState")
    end

    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"

    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])

    print("Game on state", gamestates[gs], "target is ".. gamestates[0])
    -- if not on game mode, move to game mode
    if (gamestates[gs] == gamestates[2]) then
        moveAndUpdate("select")
        menu(ram_table)
    else 
        if (gamestates[gs] ~= gamestates[0]) then
            moveAndUpdate("start")
            menu(ram_table)
        end
    end
end

M.game = game
M.menu = menu

return M