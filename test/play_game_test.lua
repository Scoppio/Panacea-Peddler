local utils = require(".utils")

local M = {}
 
local function execute(ram_table)
    
    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"

    local function readCursor(idx)
        local cursor = {}
        cursor.id = memory.readbyte(ram_table["_cursor"])
        cursor.cell = memory.readbyte(ram_table["_cursor"]+2)
        print(cursor)
        return cursor
    end

    local function getGameState()
        return gamestates[memory.readbyte(ram_table["_GameState"])]
    end

    local function moveAndUpdate(key)
        local tt = {}
        tt[key] = true
        joypad.write(1, tt)
        utils.wait(5, "testing movement")
        return readCursor()
    end

    local inputtable = {}
    local res = 0
    
    local cursor = readCursor()
    
    -- up down left right start select A B
    local gs = getGameState()
    print("Game on state", gs)

    -- if not on game mode, move to game mode
    if (gs == gamestates[2]) 
    then
        if cursor.cell == 0 then
            print("Controller on game::starts at cell 0 - sucess")
            res = res + 1
        else
            print("Controller on game::starts at cell 0 - fail - started at " .. cursor.cell)
        end

        cursor = moveAndUpdate("up")
        if cursor.cell == 4 then
            res = res + 1
            print("Controller on game::pressing UP moves to cell 3 - sucess")
        else
            print("Controller on game::pressing UP moves to cell 3 - fail - moved to " .. cursor.cell)
        end
        
        moveAndUpdate("right")
        moveAndUpdate("right")
        cursor = moveAndUpdate("right")
        if cursor.cell == 7 then
            res = res + 1
            print("Controller on game::pressing right 3 times moves to cell 7 - sucess")
        else
            print("Controller on game::pressing right 3 times moves to cell 7 - fail - moved to ".. cursor.cell)
        end

        cursor = moveAndUpdate("down")
        if cursor.cell == 3 then
            res = res + 1
            print("Controller on game::pressing down moves to cell 3 - sucess")
        else
            print("Controller on game::pressing down moves to cell 3 - fail - moved to ".. cursor.cell)
        end
        
        moveAndUpdate("left")
        moveAndUpdate("left")
        cursor = moveAndUpdate("left")
        if cursor.cell == 0 then
            res = res + 1
            print("Controller on game::pressing left 3 times moves to cell 0 - sucess")
        else
            print("Controller on game::pressing left 3 times moves to cell 0 - fail - moved to ".. cursor.cell)
        end
        
        cursor = moveAndUpdate("A")
        if cursor.id > 0 then
            res = res + 1
            print("Controller on game::pressing A puts card in cursor - sucess - " .. cursor.id)
        else
            print("Controller on game::pressing A puts card in cursor - fail - " .. cursor.id)
        end

        cursor = moveAndUpdate("B")
        if cursor.id == 0 then
            res = res + 1
            print("Controller on game::pressing B removes card from cursor - sucess - " .. cursor.id)
        else
            print("Controller on game::pressing B removes card from cursor - fail - " .. cursor.id)
        end

        moveAndUpdate("A")
        moveAndUpdate("up")
        cursor = moveAndUpdate("A")
        table1 = memory.readbyte(ram_table["_table"])
        if cursor.id == 0 then
            res = res + 1
            print("Controller on game::pressing A on cell 4 removes card from cursor and put on table - sucess - " .. cursor.id .. " table is " .. table1)
        else
            print("Controller on game::pressing A on cell 4 removes card from cursor and put on table - fail - " .. cursor.id .. " table is " .. table1)
        end

        moveAndUpdate("select")
        gs = getGameState()
        if (gs == gamestates[3]) then
            res = res + 1
            print("Controller on game::pressing select opens endscreen - sucess")
        else
            print("Controller on game::pressing select opens endscreen - fail - screen is ".. gs)
        end
    else
        print("Controller on game:: fail - scren is ".. gs)
    end

    return res, 9
end

M.execute = execute


return M