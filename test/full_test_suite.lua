local ram_debug_parser = require(".ram_debug_parser")
local validate_decks = require(".validate_decks")

local gamesramnl = "C:\\Users\\scoppio\\Documents\\Projects\\NES\\Panacea-Peddler\\output\\game.nes.ram.nl"

local ram_table = ram_debug_parser.execute(gamesramnl)
local one_second = 60

while (one_second > 0) do
    gui.text(50,50,"Loading");
    emu.frameadvance();
    one_second = one_second - 1
end

validate_decks.execute(ram_table)

while (true) do
    gui.text(50,50,"Running Tests");
    emu.frameadvance();
end