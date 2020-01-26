local ram_debug_parser = require(".ram_debug_parser")
local validate_decks = require(".validate_decks")
local menu_test = require(".menu_test")
local go_to = require(".game_start")
local play_game_test = require(".play_game_test")
local utils = require(".utils")

-- base material for scoring tests and accessing memory variables

local gamesramnl = "C:\\Users\\scoppio\\Documents\\Projects\\NES\\Panacea-Peddler\\output\\game.nes.ram.nl"
local ram_table = ram_debug_parser.execute(gamesramnl)
local test_score = {}

-- test procedure

utils.wait_a_sec("Running Tests")

go_to.menu(ram_table)

test_score.menu, test_score.menu_max = menu_test.execute(ram_table)

go_to.game(ram_table)

test_score.decks, test_score.deck_max = validate_decks.execute(ram_table)
test_score.play_card, test_score.play_card_max = play_game_test.execute(ram_table)
test_score.score_card, test_score.score_card_max = 0, 1

-- show test score
local text_menu = "menu: " .. test_score.menu .. "/" .. test_score.menu_max
local text_deck = "decks: " .. test_score.decks .. "/" .. test_score.deck_max
local text_play_card = "play card: " .. test_score.play_card .. "/".. test_score.play_card_max
local text_score_card = "score card: " .. test_score.score_card .. "/" .. test_score.score_card_max

print("Test results:")
print(text_menu)
print(text_deck)
print(text_play_card)
print(text_score_card)

while (true) do
    
    gui.text(50,50, "Tests results")
    gui.text(50,60, text_menu)
    gui.text(50,70, text_deck)
    gui.text(50,80, text_play_card)
    gui.text(50,90, text_score_card)

    emu.frameadvance();
end
