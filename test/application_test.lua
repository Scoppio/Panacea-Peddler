local utils = require(".utils")
local gint = require(".game_interface")

local deck_test = require(".deck_test")
local menu_test = require(".menu_test")
local game_controller_test = require(".game_controller_test")

-- base material for scoring tests and accessing memory variables
local t = {}

-- test procedure
utils.wait_a_sec("Running Tests")

gint.gotoMenu(true)
t.menu, t.menu_max = menu_test.execute()
gint.gotoGame(true)
utils.wait_a_sec()
t.decks, t.deck_max = deck_test.execute()
t.play_card, t.play_card_max = game_controller_test.execute()
t.score_card, t.score_card_max =  0, 1

-- show test score
local text_menu = "menu: " .. t.menu .. "/" .. t.menu_max
local text_deck = "decks: " .. t.decks .. "/" .. t.deck_max
local text_play_card = "play card: " .. t.play_card .. "/".. t.play_card_max
local text_score_card = "score card: " .. t.score_card .. "/" .. t.score_card_max

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
