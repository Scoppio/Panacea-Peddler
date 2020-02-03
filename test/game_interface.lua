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
            print(string.format("[CARD ADDR] $ %X", card_addr))
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

local function readFlag(flagname, offset)
    if offset == nil then
        offset = 0
    end
    return memory.readbyte(ram_table[flagname] + offset)
end

local function readCursor(verbose)
    utils.wait(20)
    local cursor = {}
    local hi, low = memory.readbyte(ram_table["_cursor"]+1),  memory.readbyte(ram_table["_cursor"])
    local card_addr = bit.bor(low, bit.lshift(hi,8))
    if verbose then
        print(string.format("hi: %X", hi), string.format("low: %X", low))
        print(string.format("card addr %X  at position at %X", card_addr, ram_table["_cursor"]))
    end

    cursor.card = getCard(card_addr, verbose)
    cursor.cell = memory.readbyte(ram_table["_cursor"]+2)
    
    return cursor
end

local function readCardOnTable(idx, verbose)
    local table_ptr_addr = ram_table["_table_ptr"]+(idx * 2)
    local table_addr = memory.readWord(table_ptr_addr)
    local card_addr = memory.readWord(table_addr)
    if verbose then
        print("Table pointer address ", string.format("%X", table_ptr_addr))
        print("Table pointer addressed ", string.format("%X", table_addr))
        print("Looking for card pointer on address", string.format("%X", card_addr))
    end
    return getCard(card_addr)
end

function readWord(addr)
    local hi, low = memory.readbyte(addr + 1), memory.readbyte(addr)
    return bit.bor(low, bit.lshift(hi,8))
end


local function readCardOnCell(cell, verbose)

    if cell > 3 then
        return readCardOnTable(cell-4, verbose)
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
    if verbose then
        print(addresses)
        print(cell)
        print(addresses[cell][0])
        print(ram_table[addresses[cell][0]])
    end
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
M.readFlag = readFlag

memory.readWord = readWord

return M