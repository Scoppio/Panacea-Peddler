local utils = require(".utils")
local assertions = require(".assertions")

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
        res = res + assertEquals(0, cursor.cell, "game_controller_test::start at cell 0")
        
        cursor = moveAndUpdate("up")
        res = res + assertions.assertEquals(4, cursor.cell, "game_controller_test::pressing up moves to cell 3")
        
        moveAndUpdate("right")
        moveAndUpdate("right")
        cursor = moveAndUpdate("right")
        res = res + assertions.assertEquals(7, cursor.cell, "game_controller_test::pressing right 3 times moves from cell 4 to 7")
        
        cursor = moveAndUpdate("down")
        res = res + assertions.assertEquals(3, cursor.cell, "game_controller_test::pressing down moves from cell 7 to 3")
        
        moveAndUpdate("left")
        moveAndUpdate("left")
        cursor = moveAndUpdate("left")
        res = res + assertions.assertEquals(0, cursor.cell, "game_controller_test::presing left 3 times moves from 3 to 0")
        
        cursor = moveAndUpdate("A")
        res = res + assertions.assertTrue(cursor.id > 0, "game_controller_test::pressing A puts card in cursor")

        cursor = moveAndUpdate("B")
        res = res + assertions.assertEquals(0, cursor.id, "game_controller_test::pressing B removes card from cursor")

        moveAndUpdate("A")
        moveAndUpdate("up")
        cursor = moveAndUpdate("A")
        table1 = memory.readbyte(ram_table["_table"])
        res = res + assertions.assertEquals(0, cursor.id, "game_controller_test::pressing A on cell 4 removes card from cursor and put on table")

        moveAndUpdate("select")
        gs = getGameState()
        res = res + assertions.assertEquals(gamestates[3], g2, "game_controller_test::pressing select opens endscreen")
    else
        print("Controller on game:: fail - scren is ".. gs)
    end

    return res, 9
end

M.execute = execute


return M