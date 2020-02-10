local utils = require(".utils")
local gint = require(".game_interface")

-- base material for scoring tests and accessing memory variables

function asStr(v)
    if v then
        return v..""
    end
    return "nil"
end

function cardIdOnCell(c, verbose)
    local card = gint.readCardOnCell(c, verbose)
    if card then
        return asStr(card.id)
    end
    return "nil"
end

while (true) do
    local msg =  "" .. asStr(gint.getGameState())
    local cursor = gint.readCursor()
    local m2 = "score: "..gint.readFlag("_round_score")
    local m3 = "round: "..gint.readFlag("_round")
    gui.text(5,20,msg)
    gui.text(5,40,"cursor at cell " .. cursor.cell)
    if cursor.card then    
        local card = cursor.card
        local txt = "Card [id="..asStr(card.id).." value="..asStr(card.value).." color="..asStr(card.color)..".."
        local txt2 = " Lmodifier="..asStr(card.Lmodifier).." Rmodifier="..asStr(card.Rmodifier).."]"
        gui.text(5,60,txt)
        gui.text(5,70,txt2)
    end
    gui.text(5,80,m2)
    gui.text(5,90,m3)
    local c0 = cardIdOnCell(0)
    local c1 = cardIdOnCell(1)
    local c2 = cardIdOnCell(2)
    local c3 = cardIdOnCell(3)
    local c4 = cardIdOnCell(4, true)
    local c5 = cardIdOnCell(5, true)
    local c6 = cardIdOnCell(6, true)
    local c7 = cardIdOnCell(7, true)
    
    local m5 = c4.."|"..c5.."|"..c6.."|"..c7    
    gui.text(50,100, "Tab: "..m5)
    
    local m4 = c0.."|"..c1.."|"..c2.."|"..c3
    gui.text(50,110, "Dec: "..m4)

    emu.frameadvance();
end

