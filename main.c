/*  Panacea Pedler
 *  @author scoppio
 *  @date 2019-2020
 */
#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/colornames.h"

#include "lib/sounds.h"
#include "lib/registers.h"
#include "main.h"
#include "sprites.h"
#include "cheats.h"

// there's some oddities in the palette code, black must be 0x0f, white must be 0x30

#pragma bss - name(push, "ZEROPAGE")

// Game states
#define MENU 0
#define MENU_SETTINGS 1
#define GAME 2
#define ENDSCREEN 3

// GLOBAL VARIABLES
// all variables should be global for speed
// zeropage global is even faster

unsigned char GameState = MENU;

#define SCREEN_POS(x, y) (vram_adr(NTADR_A(x, y)))
#define PRINT(t) (vram_write(t, sizeof(t)))
#define PRINT_AT(x, y, t) \
	SCREEN_POS(x, y);     \
	PRINT(t);             \
	SCREEN_POS(0, 0);

#define blue_size_pt cards_size_ptr[0]
#define green_size_pt cards_size_ptr[1]
#define yellow_size_pt cards_size_ptr[2]
#define red_size_pt cards_size_ptr[3]

#define BTN(btn) (pad1_new & btn)
#define HEX_TO_CHAR(byt) (((byt & 0x0F) > 0x9) ? (byt & 0x0F) + SEVEN_CHAR : (byt & 0x0F) + ZERO_CHAR)
#define BLUE_IDX (blue_idx[blue_size_pt])
#define GREEN_IDX (green_idx[green_size_pt])
#define YELLOW_IDX (yellow_idx[yellow_size_pt])
#define RED_IDX (red_idx[red_size_pt])
#define RANDOM_MODIFIER (rand8() & 0x3)

void _init(void)
{
	ppu_off();			 // screen off
	pal_bg(palette_bg);  //	load the BG palette
	pal_spr(palette_sp); // load the Sprite palette
	ppu_on_all();		 //	turn on screen
	bank_spr(1);
	draw_bg();
	reset_game();
}

void reset_game(void)
{
	cursor.card = NULL;
	cursor.cell = 0;
	table[0] = NULL;
	table[1] = NULL;
	table[2] = NULL;
	table[3] = NULL;
	
	red_size_pt = 0;
	yellow_size_pt = 0;
	green_size_pt = 0;
	blue_size_pt = 0;

	red_bc_count = 3;
	yellow_bc_count = 3;
	green_bc_count = 3;
	blue_bc_count = 3;
	round = 13;
	round_score = 0;

	shuffle_decks();
}

void _update60(void)
{
	// do nothing atm;
	controller();
	end_of_round();
}

/* End of turn
* Verifies if the table is complete
* and if it is, it checks for score
* and change the round.
* If the score is 0 or less than it
* is a loss and the game ends.
* if the round reaches 0 the game ends.
*/
void end_of_round(void) {
	if (GameState != GAME || 
		table[0] == NULL || 
		table[1] == NULL || 
		table[2] == NULL || 
		table[3] == NULL)
	{
		return;
	}

	w = count_points();
	if (i > 0) {
		round_score += w;
		round--;
	}
	if (round == 0 || w < 0) {
		GameState = ENDSCREEN;
	}
	if (cheat_num == CHEAT_RESET_ROUND) {
		reset_game();
		cheat_num = CHEAT_DISABLED;
	}
}

int count_points(void) {
	int val = 0;
	int temp = 0;
	for (i=0; i<4; i++) {
		temp = table[i]->value;
		if (i > 0) {
			if (table[i]->Lmodifier == table[i-1]->color) {
				val+=temp;
			}
		}
		if (i < 3) {
			if (table[i]->Rmodifier == table[i+1]->color) {
				val+=temp;
			}
		}
		val += temp;
	}

	return val;
}

void shuffle_decks(void)
{
	shuffle(&red_idx);
	shuffle(&blue_idx);
	shuffle(&green_idx);
	shuffle(&yellow_idx);
	instantiate_card_modifiers(red_cards);
	instantiate_card_modifiers(blue_cards);
	instantiate_card_modifiers(green_cards);
	instantiate_card_modifiers(yellow_cards);
}

void shuffle(unsigned char (*array)[13])
{
	for (i = 0; i < DECK_CARDS_SIZE; i++)
	{
		do
		{
			j = i + rand8() & 0x0F;
		} while (j >= DECK_CARDS_SIZE);
		t = (*array)[j];
		(*array)[j] = (*array)[i];
		(*array)[i] = t;
	}
}

void instantiate_card_modifiers(struct Card * cards)
{
	for (i = 0; i < DECK_CARDS_SIZE; i++) {
		if (cards[i].Lmodifier != M_NONE) {
			cards[i].Lmodifier = RANDOM_MODIFIER;
		}
		if (cards[i].Rmodifier != M_NONE) {
			cards[i].Rmodifier = RANDOM_MODIFIER;
		}
	}
}

/*	Interact with table
*
*	If you have a card in the cursor, it will release 
*	the card and push it back to its original deck
*/
void interact_with_table()
{
	if (cursor.cell < 4 && cursor.card == NULL) {
		// pickup card
		switch (cursor.cell)
		{
			case 0:
				if (blue_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &blue_cards[BLUE_IDX];
					blue_size_pt++;
				}
				break;
			case 1:
				if (green_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &green_cards[GREEN_IDX];
					green_size_pt++;
				}
				break;
			case 2:
				if (yellow_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &yellow_cards[YELLOW_IDX];
					yellow_size_pt++;
				}
				break;
			case 3:
				if (red_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &red_cards[RED_IDX];
					red_size_pt++;
				}
				break;
		}
	} else {
		// place card on table
		i = cursor.cell-4;
		if (cursor.cell > 3 && table[i] == NULL) {
			table[i] = cursor.card;
			cursor.card = NULL;
		} else {
			play_sound(snd_ILLEGAL_ACTION);
		}
	}
}

/*	Cancel Card
*
*	If you have a card in the cursor, it will release 
*	the card and push it back to its original deck
*/
void cancel_card() 
{
	if (cursor.card == NULL) return;
	i = (cursor.card->id >> 4);
	cards_size_ptr[i]--;
	cursor.card = NULL;
}

void controller(void) {
	pad_poll(0);			   // read the first pad
	pad1_new = get_pad_new(0); // newly pressed button. do pad_poll first

	switch (GameState)
	{
		case MENU:
			controller_menu();
			break;
		case MENU_SETTINGS:
			controller_menu_settings();
			break;
		case GAME:
			controller_game();
			break;
		case ENDSCREEN:
			controller_endscreen();
			break;
		default:
			GameState = MENU;
			break;
	}
}

void controller_menu_settings(void)
{
	if (BTN(PAD_START)) 
	{
		GameState = MENU;
	}
}

/**
 *
 *  Table (table and its cell position)
 *  [0->4, 1->5, 2->6, 3->7]
 * 
 *  Cells
 *  | 4 | 5 | 6 | 7 |
 *  | 0 | 1 | 2 | 3 |
 * 
 *  0 = blue
 *  1 = green
 *  2 = yellow
 *  3 = red
 * 
 *  4 = place_card
 *  5 = place_card
 *  6 = place_card
 *  7 = place_card
 */
void controller_game(void)
{
	if (BTN(PAD_A))
	{
		// if the cell is under 4 it will place on the table
		// therefore it will place the card on the table at the cell it is targeting.
		interact_with_table();
	}
	if (BTN(PAD_B))
	{
		cancel_card();
	}
	if (BTN(PAD_LEFT))
	{
		cursor.cell = cursor.cell == 0 ? 3 : cursor.cell == 4 ? 7 : cursor.cell - 1;
		// cursor.cell = cursor.cell-1 & 0x03;
	}
	if (BTN(PAD_RIGHT))
	{
		
        cursor.cell = cursor.cell == 3 ? 0 : cursor.cell == 7 ? 4 : cursor.cell + 1;
		// cursor.cell = cursor.cell+1 & 0x03;
	}
	if (BTN(PAD_UP))
	{
		cursor.cell |= 0x4;
	}
	if (BTN(PAD_DOWN))
	{
		cursor.cell &= 0x3;
	}
	if (BTN(PAD_SELECT))
	{
		GameState = ENDSCREEN;
	}
}

void controller_endscreen(void)
{
	if (BTN(PAD_A) || BTN(PAD_B) || BTN(PAD_START)) 
	{
		GameState = MENU;
	}
	// setup best_scores
	for (j = 0; j < 5; j++)
	{
		w = best_scores[j];
		convert_w_to_decimal();
		
		score_text[0] = 'L' + j;
		score_text[1] = 'S' + j;
		score_text[2] = 'C' + j;
		score_text[6] = thousands;
		score_text[7] = hundreds;
		score_text[8] = tens;
		score_text[9] = ones;
		
		PRINT_AT(10, 3 + j*2, score_text);
	}
}

void controller_menu(void)
{
	
	if (BTN(PAD_START)) 
	{
		GameState = GAME;
		reset_game();
	} else if (BTN(PAD_SELECT)) 
	{
		// menu
		GameState = MENU_SETTINGS;
	}
}

void _draw(void)
{
	ppu_wait_nmi();
	timer_draw();
	
	//draw_card_piles();
	//draw_cards_table();
}

void timer_draw(void)
{
	i = second;
	convert_i_to_decimal();
	datetime[7] = ones;
	datetime[6] = tens;
	i = minute;
	convert_i_to_decimal();
	datetime[4] = ones;
	datetime[3] = tens;
	i = hour;
	
	convert_i_to_decimal();
	datetime[1] = ones;
	datetime[0] = tens;
	PRINT_AT(10, 2, datetime);
}


void draw_sprites(void)
{
	// clear all sprites from sprite buffer
	oam_clear();

	// draw 2 metasprites
	oam_meta_spr(28, 120, PaddleSpr);
	oam_meta_spr(47, 97, BallSpr);
}

void draw_bg(void)
{
	// ppu_off(); // screen off

	// vram_adr(NAMETABLE_A);
	// // this sets a start position on the BG, top left of screen

	// vram_unrle(breaky_bg2);
	// // this unpacks a compressed full nametable

	// memcpy(c_map, c1, 256);
	// // copy the collision map to c_map

	// vram_adr(NTADR_A(0, 6));
	// // sets a start address, skipping the top of the screen

	// for (temp_y = 0; temp_y < 16; ++temp_y)
	// {
	// 	for (temp_x = 0; temp_x < 16; ++temp_x)
	// 	{
	// 		temp1 = (temp_y << 4) + temp_x;
	// 		if ((temp_x == 0) || (temp_x == 15))
	// 		{
	// 			vram_put(0x10); // wall at the edges
	// 			vram_put(0x10);
	// 		}
	// 		else
	// 		{
	// 			if (c_map[temp1])
	// 			{					// if block = yes
	// 				vram_put(0x11); // draw block
	// 				vram_put(0x12);
	// 			}
	// 			else
	// 			{
	// 				vram_put(0); // else draw blank
	// 				vram_put(0);
	// 			}
	// 		}
	// 	}
	// }

	// ppu_on_all();
}





void main(void)
{
	_init();
	_game_loop();
}

void _game_loop(void)
{
	while (1)
	{
		_clock_counter();
		_update60();
		_draw();
	}
}