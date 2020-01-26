local utils = require(".utils")
local assertions = require(".assertions")
local gint = require(".game_interface")

local M = {}
 
local function execute(ram_table)

    local cursor = gint.readCursor()
    assertions.createTestContext("game_controller_test")
    
    assertions.assertEquals(0, gint.readCursor().cell, "start at cell 0")
    
    gint.input("up")
    assertions.assertEquals(4, gint.readCursor().cell, "pressing up moves to cell 3")
    
    gint.input("right")
    gint.input("right")
    gint.input("right")
    assertions.assertEquals(7, gint.readCursor().cell, "pressing right 3 times moves from cell 4 to 7")
    
    gint.input("down")
    assertions.assertEquals(3, gint.readCursor().cell, "pressing down moves from cell 7 to 3")
    
    gint.input("left")
    gint.input("left")
    gint.input("left")
    assertions.assertEquals(0, gint.readCursor().cell, "presing left 3 times moves from 3 to 0")
    
    local card = gint.readCardOnTable(0)
    gint.input("A")
    assertions.assertTrue(card.id, gint.readCursor().id, "pressing A puts card from table in cursor")

    gint.input("B")
    assertions.assertEquals(0, gint.readCursor().id, "pressing B removes card from cursor")

    gint.input("A")
    cursor = gint.readCursor()
    gint.input("up")
    gint.input("A")
    card = gint.readCardOnTable(0)
    assertions.assertEquals(0, gint.readCursor().id, "pressing A on cell 4 removes card from cursor")
    assertions.assertTrue(cursor.id, card.id, "pressing A on cell 4 put card on table")

    gint.input("select")
    assertions.assertEquals(gamestates[3], gint.getGameState(), "pressing select opens endscreen")

    return assertions.getPassed(), assertions.getTotal()
end

M.execute = execute


return M