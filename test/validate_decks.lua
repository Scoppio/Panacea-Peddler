local M = {}
 
local function execute(ram_table)
    
    local function readCursor(idx)
        return memory.readbyte(ram_table["_cursor"]+idx)
    end
    local res = 0;
    print("Deck Shuffling Test")
    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])
    print("assert GameState == 2 ? ", gs == 2, " - ", gs)
    
    local cursor = {}
    cursor.id = readCursor(0)
    cursor.value = readCursor(1)
    cursor.color = readCursor(2)
    cursor.Lmodifier = readCursor(3)
    cursor.Rmodifier = readCursor(4)
    cursor.cell = readCursor(5)
    
    print(cursor)
    -- has 6 chars, first 5 are a card, last one is the cell

    return res, 8
end
M.execute = execute
 
return M