local M = {}
 
local function wait_a_sec(text)
    local one_second = 60;
    while (one_second > 0) do
        if text then
            gui.text(50,50,text);
        end
        emu.frameadvance();
        one_second = one_second - 1
    end
end

local function wait(time, text)

    local tt = time;
    while (tt > 0) do
        if text then
            gui.text(50,50,text);
        end
        emu.frameadvance();
        tt = tt - 1
    end
end

M.wait_a_sec = wait_a_sec
M.wait = wait
 
return M