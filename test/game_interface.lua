local utils = require(".utils")
local ram_debug_parser = require(".ram_debug_parser")
local gamesramnl = "C:\\Users\\scoppio\\Documents\\Projects\\NES\\Panacea-Peddler\\output\\game.nes.ram.nl"
local ram_table = ram_debug_parser.execute(gamesramnl)

local M = {}

local function getRamTable()
    return ram_table
end

gamestates = {}
gamestates[0] = "MENU"
gamestates[1] = "MENU_SETTINGS"
gamestates[2] = "GAME"
gamestates[3] = "ENDSCREEN"

function gameStates()
    local gamestates = {}
    gamestates[0] = "MENU"
    gamestates[1] = "MENU_SETTINGS"
    gamestates[2] = "GAME"
    gamestates[3] = "ENDSCREEN"
    return gamestates
end

card_colors = {}
card_colors[0] = "BLUE"
card_colors[1] = "GREEN"
card_colors[2] = "YELLOW"
card_colors[3] = "RED"
card_colors[5] = "BLACK"

local function readCursor()
    local cursor = {}
    local card_addr = memory.readbyte(ram_table["_cursor"])
    cursor.cell = memory.readbyte(ram_table["_cursor"]+2)

    cursor.id = memory.readbyte(card_addr)
    cursor.value = memory.readbyte(card_addr+1)
    cursor.color = card_colors[memory.readbyte(card_addr+2)]
    cursor.Lmodifier = card_colors[memory.readbyte(card_addr+3)]
    cursor.Rmodifier = card_colors[memory.readbyte(card_addr+4)]

    return cursor
end

local function readCardOnTable(idx)
    local card_addr = memory.readbyte(ram_table["_table"] + idx)
    
    local card = {}
    card.id = memory.readbyte(card_addr)
    card.value = memory.readbyte(card_addr+1)
    card.color = card_colors[memory.readbyte(card_addr+2)]
    card.Lmodifier = card_colors[memory.readbyte(card_addr+3)]
    card.Rmodifier = card_colors[memory.readbyte(card_addr+4)]

    return card
end

local function getGameState()
    local gs = memory.readbyte(ram_table["_GameState"])
    return gameStates()[gs]
end

local function input(key)
    local tt = {}
    tt[key] = true
    joypad.write(1, tt)
    utils.wait(5)
end

local function gotoGame()
    local gs = getGameState()
    print("Game on state ".. gs .. " target is ".. gameStates()[2])

    -- if not on game mode, move to game mode
    if (gs == gameStates()[2]) 
    then
        return
    else
        input("start", "Current mode = ".. gs)
        gotoGame()
    end
end

local function gotoMenu()
    local gs = getGameState()
    print("Game on state " .. gs .. " target is " .. gameStates()[2])
    -- if not on game mode, move to game mode
    if (gs == gameStates()[2]) then
        input("select")
        gotoMenu()
    else 
        if (gs ~= gameStates()[0]) then
            input("start")
            gotoMenu()
        end
    end
end

M.gotoGame = gotoGame
M.gotoMenu = gotoMenu
M.input = input
M.getGameState = getGameState
M.getRamTable = getRamTable
M.readCursor = readCursor
M.readCardOnTable = readCardOnTable

return M