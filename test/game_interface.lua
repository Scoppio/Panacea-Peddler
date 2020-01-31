local utils = require(".utils")
local ram_debug_parser = require(".ram_debug_parser")
local gamesramnl = "C:\\Users\\scoppio\\Documents\\Projects\\NES\\Panacea-Peddler\\output\\game.nes.ram.nl"
local ram_table = ram_debug_parser.execute(gamesramnl)

local M = {}

local function getRamTable()
    return ram_table
end

local function gameStates()
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

local function getCard(card_addr, verbose)
    local card = {}
    if card_addr > 0 then
        if verbose then
            print("[CARD ADDR] $", card_addr)
        end
        card.id = memory.readbyte(card_addr)
        card.value = memory.readbyte(card_addr+1)
        card.color = card_colors[memory.readbyte(card_addr+2)]
        card.Lmodifier = card_colors[memory.readbyte(card_addr+3)]
        card.Rmodifier = card_colors[memory.readbyte(card_addr+4)]
    else
        card = nil
    end
    return card
end

local function setFlag(flagname, value)
    memory.writebyte(ram_table[flagname], value)
end

local function readCursor(verbose)
    local cursor = {}
    local card_addr = memory.readbyte(ram_table["_cursor"])
    if verbose then
        print(card_addr .. " at position " .. ram_table["_cursor"])
    end
    cursor.cell = memory.readbyte(ram_table["_cursor"]+2)
    cursor.card = getCard(card_addr, verbose)
    return cursor
end

local function readCardOnTable(idx, verbose)
    if verbose then
        print("Looking for card pointer on address" + ram_table["_table"] + (idx*2))
    end
    local card_addr = memory.readbyte(ram_table["_table"] + (idx * 2))
    return getCard(card_addr)
end


local function readCardOnCell(cell, verbose)

    if cell > 3 then
        return readCardOnTable(cell, verbose)
    end
    --  0 = blue
    --  1 = green
    --  2 = yellow
    --  3 = red
    local addresses = {}
    addresses[0] = {}
    addresses[0][0]= "_blue_cards"
    addresses[0][1]= "_blue_idx"
    addresses[1] = {}
    addresses[1][0]= "_green_cards"
    addresses[1][1]= "_green_idx"
    addresses[2] = {}
    addresses[2][0]= "_yellow_cards"
    addresses[2][1]= "_yellow_idx"
    addresses[3] = {}
    addresses[3][0]= "_red_cards"
    addresses[3][1]= "_red_idx"

    local pointer = nil
    
    local deck_addr = ram_table[addresses[cell][0]]
    local idx_pos = memory.readbyte(ram_table["_cards_size_ptr"]+cell)
    local idx_addr = memory.readbyte(ram_table[addresses[cell][1]]+idx_pos)
    local card = getCard(deck_addr + (idx_addr*5))
    if verbose then
        print("At position " .. deck_addr + (idx_addr*5) .. " was found a card ", card)
    end
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

local function gotoGame(verbose)
    local gs = getGameState()
    if verbose then
        print("Game on state ".. gs .. " target is ".. gameStates()[2])
    end
    -- if not on game mode, move to game mode
    if (gs == gameStates()[2]) 
    then
        return
    else
        input("start", "Current mode = ".. gs)
        gotoGame()
    end
end

local function gotoMenu(verbose)
    local gs = getGameState()
    if verbose then
        print("Game on state " .. gs .. " target is " .. gameStates()[2])
    end
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
M.gameStates = gameStates
M.getGameState = getGameState
M.getRamTable = getRamTable
M.readCursor = readCursor
M.readCardOnTable = readCardOnTable
M.setFlag = setFlag
M.readCardOnCell = readCardOnCell 

return M