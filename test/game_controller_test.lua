local utils = require(".utils")
local assertions = require(".assertions")
local gint = require(".game_interface")

local M = {}
 
local function execute(ram_table)

    local cursor = gint.readCursor()
    assertions.createTestContext("game_controller_test")
    assertions.assertEquals(0, gint.readCursor().cell, "start at cell 0")
    
    gint.input("up")
    assertions.assertEquals(4, gint.readCursor().cell, "pressing up moves to cell 4")
    gint.input("right")
    assertions.assertEquals(5, gint.readCursor().cell, "pressing right 3 times moves from cell 4 to 5")
    gint.input("right")
    assertions.assertEquals(6, gint.readCursor().cell, "pressing right 3 times moves from cell 5 to 6")
    gint.input("right")
    assertions.assertEquals(7, gint.readCursor().cell, "pressing right 3 times moves from cell 6 to 7")
    
    gint.input("down")
    assertions.assertEquals(3, gint.readCursor().cell, "pressing down moves from cell 7 to 3")
    
    gint.input("left")
    gint.input("left")
    gint.input("left")
    assertions.assertEquals(0, gint.readCursor().cell, "presing left 3 times moves from 3 to 0")
    gint.input("left")
    assertions.assertEquals(3, gint.readCursor().cell, "presing left 1 time moves from 0 to 3 (wraparound)")
    gint.input("right")
    assertions.assertEquals(0, gint.readCursor().cell, "presing right 1 time moves from 3 to 0 (wraparound)")

    local card = gint.readCardOnCell(gint.readCursor().cell, true)
    gint.input("A")
    assertions.assertEquals(card, gint.readCursor(true).card, "pressing A puts card from table in cursor")

    gint.input("B")
    assertions.assertEquals(nil, gint.readCursor(true).card, "pressing B removes card from cursor")

    gint.input("A")
    assertions.assertNotNull(gint.readCursor(true).card, "cursor must have a card")
    print("cursor card", gint.readCursor().card)
    card = gint.readCursor().card
    gint.input("up")
    assertions.assertEquals(4, gint.readCursor().cell, "Pressing up moves to cell 4")
    local before = gint.readCursor().card
    gint.input("A")
    assertions.assertNotEquals(before, gint.readCursor().card, "pressing A on cell 4 removes card from cursor")    
    print("cursor card", gint.readCursor().card)
    assertions.assertEquals(card, gint.readCardOnTable(gint.readCursor().cell-4), "pressing A on cell 4 put card on table")

    print(cursor.card, card)

    gint.input("select")
    assertions.assertEquals(gint.gameStates()[3], gint.getGameState(), "pressing select opens endscreen")

    return assertions.getPassed(), assertions.getTotal()
end

M.execute = execute


return M