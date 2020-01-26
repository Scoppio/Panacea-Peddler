local M = {}
 
local function execute(ram_table)
    
    local function readCursor(idx)
        local cursor = {}
        cursor.id = memory.readbyte(ram_table["_cursor"])
        cursor.cell = memory.readbyte(ram_table["_cursor"]+2)
        return cursor
    end

    local res = 0;
    print("Deck Shuffling Test")
    -- up down left right start select A B
    local gs = memory.readbyte(ram_table["_GameState"])
    print("assert GameState == 2 ? ", gs == 2, " - ", gs)
    
    local cursor = {}
    local cursor = readCursor()
    
    memory.readbyte(ram_table["_GameState"])

    return res, 8
end
M.execute = execute
 
return M