local M = {}

local function execute(file)
    
    -- see if the file exists
    function file_exists(file)
        local f = io.open(file, "rb")
        if f then f:close() end
        return f ~= nil
    end

    -- get all lines from a file, returns an empty 
    -- list/table if the file does not exist
    function lines_from(file)
        if not file_exists(file) then return {} end
        lines = {}
        for line in io.lines(file) do 
        lines[#lines + 1] = line
        end
        return lines
    end

    print("Loading ram file")
        
    -- tests the functions above
    local lines = lines_from(file)
    
    local ram_table = {}
    -- print all line numbers and their contents
    for k,v in pairs(lines) do
        local label = v:sub(7, v:len()-1)
        local line = k
        local addr = v:sub(2,5)
        print('line[' .. k .. ']', addr, "-", label)
        ram_table[label] = tonumber(addr, 16)
    end
    
    return ram_table
end
M.execute = execute
 
return M