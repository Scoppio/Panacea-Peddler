/*  Panacea Pedler
 *  @author scoppio
 *  @date 2019-2020
 */
// #define DEBUG
// #define UNITTEST


#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/colornames.h"

#include "lib/sounds.h"
#include "lib/registers.h"
#include "assets/maps/gamescene.h"
#include "assets/maps/gamecards.h"

#ifndef MAIN
#define MAIN
#include "main.h"
#endif

#include "cheats.h"

#ifdef UNITTEST
#include "unittest.h"
#endif
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

#define blue_size_pt cards_size_ptr[0]
#define green_size_pt cards_size_ptr[1]
#define yellow_size_pt cards_size_ptr[2]
#define red_size_pt cards_size_ptr[3]
#define __CARD(x) (*table_ptr)[x]

#define BTN(btn) (pad1_new & btn)
#define HEX_TO_CHAR(byt) (((byt & 0x0F) > 0x9) ? (byt & 0x0F) + SEVEN_CHAR : (byt & 0x0F) + ZERO_CHAR)
#define BLUE_IDX (blue_idx[blue_size_pt])
#define GREEN_IDX (green_idx[green_size_pt])
#define YELLOW_IDX (yellow_idx[yellow_size_pt])
#define RED_IDX (red_idx[red_size_pt])
#define RANDOM_MODIFIER (rand8() & 0x3)
#define OFFSET_I(a) (a + i)
#define OFFSET_J(a) (a + j)
#define OFFSET_N(a) (a + n)

void _init(void)
{
	ppu_off();			 // screen off
	pal_bg(palette_bg);  //	load the BG palette
	pal_spr(palette_sp); // load the Sprite palette
	ppu_on_all();		 //	turn on screen
	set_vram_buffer();	 // PPU pointed to VRAM Buffer
	bank_spr(0);		 // set bank for sprite
	reset_game();		 // reset game variables
}

void reset_game(void)
{
	cursor.card = NULL;
	cursor.cell = 0;
	(*table_ptr)[0] = NULL;
	(*table_ptr)[1] = NULL;
	(*table_ptr)[2] = NULL;
	(*table_ptr)[3] = NULL;
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
	pp = 0;
	second_forever = 0;
	map_registers = NULL;
	challenge = 0;
	kicked_client = FALSE;
	
	shuffle_decks();
}

void _update60(void)
{
	// do nothing atm;
	controller();
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
		(*table_ptr)[0] == NULL || 
		(*table_ptr)[1]  == NULL || 
		(*table_ptr)[2]  == NULL || 
		(*table_ptr)[3]  == NULL)
	{
		return;
	}
	sleep(30);
	pp = count_points();

	if (pp > 0) {
		round_score += (pp & 0x00FF);
		round--;
	}
	
	(*table_ptr)[0] = NULL;
	(*table_ptr)[1] = NULL;
	(*table_ptr)[2] = NULL;
	(*table_ptr)[3] = NULL;
	i = 0;

	challenge = (challenge + 1) & 0x0f;
	
	// old alternative way without using arrays
	// do {
	// 	i = rand8() % 4;
	// 	i = i == 4 ? BLACK_CARD : i;
	// } while (i == challenge);
	// challenge = i;
	// i = 0;
	// do {
	// 	i = rand8() % 4;
	// 	i = i == 4 ? BLACK_CARD : i;
	// } while (i == challenge);
	// preferred = i;

	if (round == 0 || pp <= 0) {
		GameState = ENDSCREEN;
	} else {
		update_reg = update_reg | SCORE_UPDATED | CLIENT_UPDATED;
	}

	if (cheat_num == CHEAT_RESET_ROUND) {
		reset_game();
		cheat_num = CHEAT_DISABLED;
	}
}

/* Count Points
* This method looks for the total number of 
* points the cards total to and return it
*
* It also take into consideration things like
* prefered cards, required cards, bonus modifiers
*/


signed char count_points(void) {
	sj = 0;
	si = 0;
	j = FALSE;
	// leftmost card
	si = __CARD(0)->value;
	if (__CARD(0)->Rmodifier == __CARD(1)->color) {
		si += __CARD(0)->value;
	}
	if (__CARD(0)->color == preferred_table[challenge]) {
		si += si;
	}
	sj += si;
	if (__CARD(0)->color == challenge_table[challenge]) {
		j = TRUE;
	}

	si = __CARD(1)->value;
	if (__CARD(1)->Lmodifier == __CARD(0)->color) {
		si += __CARD(1)->value;
	}
	if (__CARD(1)->Rmodifier == __CARD(2)->color) {
		si += __CARD(1)->value;
	}
	if (__CARD(1)->color == preferred_table[challenge]) {
		si += si;
	}
	sj += si;
	if (__CARD(1)->color == challenge_table[challenge]) {
		j = TRUE;
	}

	si = __CARD(2)->value;
	if (__CARD(2)->Lmodifier == __CARD(1)->color) {
		si += __CARD(2)->value;
	}
	if (__CARD(2)->Rmodifier == __CARD(3)->color) {
		si += __CARD(2)->value;
	}
	if (__CARD(2)->color == preferred_table[challenge]) {
		si += si;
	}
	sj += si;
	if (__CARD(2)->color == challenge_table[challenge]) {
		j = TRUE;
	}

	si = __CARD(3)->value;
	if (__CARD(3)->Lmodifier == __CARD(2)->color) {
		si += __CARD(3)->value;
	}
	if (__CARD(3)->color == preferred_table[challenge]) {
		si += si;
	}
	sj += si;
	if (__CARD(3)->color == challenge_table[challenge]) {
		j = TRUE;
	}
	

	if (j == FALSE) {
		sj = 0;
	}
	return sj;
}

void shuffle_decks(void)
{
	set_rand(tick);
	shuffle(&red_idx);
	shuffle(&blue_idx);
	shuffle(&green_idx);
	shuffle(&yellow_idx);
	instantiate_card_modifiers(red_cards);
	instantiate_card_modifiers(blue_cards);
	instantiate_card_modifiers(green_cards);
	instantiate_card_modifiers(yellow_cards);
	challenge = rand8()&0x0f;
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

/* Uses I as argument, returns in variable temp_card */
void get_card_on_deck(void)
{
	if (i == BLUE_CARD) {
		temp_card = &blue_cards[BLUE_IDX];
	} else if (i == GREEN_CARD) {
		temp_card = &green_cards[GREEN_IDX];
	} else if (i == YELLOW_CARD) {
		temp_card = &yellow_cards[YELLOW_IDX];
	} else if (i == RED_CARD) {
		temp_card = &red_cards[RED_IDX];
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
		update_reg |= CARD_ON_TABLE_UPDATED;
	} else {
		// place card on table
		if (cursor.cell - 4 >= 0 && cursor.cell -4 < 4 && (*table_ptr)[cursor.cell-4] == NULL) {
			(*table_ptr)[cursor.cell-4] = cursor.card;
			cursor.card = NULL;
			placed_on_table = cursor.card->id;
			update_reg |= CARD_ON_TABLE_UPDATED;
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
	update_reg |= CARD_ON_TABLE_UPDATED;
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
	}
	if (BTN(PAD_RIGHT))
	{
        cursor.cell = cursor.cell == 3 ? 0 : cursor.cell == 7 ? 4 : cursor.cell + 1;
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
	
	if (BTN(PAD_START))
	{
		// kick client to end
		if ((*table_ptr)[0] == NULL && 
			(*table_ptr)[1]  == NULL && 
			(*table_ptr)[2]  == NULL && 
			(*table_ptr)[3]  == NULL && 
			kicked_client == FALSE) 
		{
			if (cursor.card != NULL) 
			{
				cancel_card();
			}
			challenge++;
			kicked_client = TRUE;
			play_sound(snd_KICK_CLIENT);
		}
	}
}

void controller_endscreen(void)
{
	if (BTN(PAD_A) || BTN(PAD_B) || BTN(PAD_START)) 
	{
		GameState = MENU;
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
	clear_vram_buffer();
	switch (GameState) {
		case MENU:
			print_entry();
			break;
		case MENU_SETTINGS:
			print_menu();
			break;
		case ENDSCREEN:
			print_scores();
			break;
		case GAME:
			print_table();
			break;
	}

	ppu_wait_nmi();
}

void _cleanup(void)
{
	end_of_round();
}

void print_table(void)
{
	
	if (map_registers != GAME_DRAWN) {
		load_room();
	}
#ifdef DEBUG
	print_debug_game_drawn()
#endif
	update_card_count();
	update_score_header();
	ppu_wait_nmi();

	update_cards_on_table();
	ppu_wait_nmi();
}

void print_debug_game_drawn(void) {
	for (j = 0; j < 8; j++) 
	{
		if (j > 3) 
		{
			n = j - 4;
			if ((*table_ptr)[n] != NULL) {
				i = (*table_ptr)[n]->id;
				convert_i_to_decimal();
			} else {
				ones = HIFEN_CHAR;
				tens = HIFEN_CHAR;
			}
			table_debug_text[3*n+2] = ones;
			table_debug_text[3*n+1] = tens;
			table_debug_text[3*n] = cursor.cell == j ? '>' : ' ';
		} else {
			i = j;
			get_card_on_deck();
			if (temp_card != NULL) {
				i = temp_card->id;
				convert_i_to_decimal();
			} else {
				ones = HIFEN_CHAR;
				tens = HIFEN_CHAR;
			}
			deck_debug_text[3*j+2] = ones;
			deck_debug_text[3*j+1] = tens;
			deck_debug_text[3*j] = cursor.cell == j ? '>' : ' ';
		}
	}
	
	cursor_text[3] = ZERO_CHAR + cursor.cell;
	
	if (cursor.card != NULL) {
		i = cursor.card->id;
		convert_i_to_decimal();
	} else {
		ones = HIFEN_CHAR;
		tens = HIFEN_CHAR;
	}
	cursor_text[11] = tens;
	cursor_text[12] = ones;

	
	multi_vram_buffer_horz(table_debug_text, sizeof(table_debug_text), NTADR_A(8, 8));
	multi_vram_buffer_horz(deck_debug_text, sizeof(deck_debug_text), NTADR_A(8, 9));
	multi_vram_buffer_horz(cursor_text, sizeof(cursor_text), NTADR_A(8, 10));
}

void update_card_count(void)
{
	if (update_reg & CARD_ON_TABLE_UPDATED) {
		i = 13-blue_size_pt;
		convert_i_to_decimal();
		cards_count[0] = tens;
		cards_count[1] = ones;
		i = 13-green_size_pt;
		convert_i_to_decimal();
		cards_count[4] = tens;
		cards_count[5] = ones;
		i = 13-yellow_size_pt;
		convert_i_to_decimal();
		cards_count[8] = tens;
		cards_count[9] = ones;
		i = 13-red_size_pt;
		convert_i_to_decimal();
		cards_count[12] = tens;
		cards_count[13] = ones;
		multi_vram_buffer_horz(cards_count, sizeof(cards_count), NTADR_A(9, 26));
		// TODO paint the back of the card that will be placed on the table
	}
}

void update_score_header(void)
{
	// last score
	if (update_reg & SCORE_UPDATED) {
		update_reg &= 0xfb;
		i = pp;
		convert_i_to_decimal();
		round_text[0] = thousands;
		round_text[1] = tens;
		round_text[2] = ones;
		multi_vram_buffer_horz(round_text, sizeof(round_text), NTADR_A(24, 2));
		
		// round total score
		i = round_score;
		convert_i_to_decimal();
		round_text[0] = thousands;
		round_text[1] = tens;
		round_text[2] = ones;
		
		multi_vram_buffer_horz(round_text, sizeof(round_text), NTADR_A(24, 1));
	}
	// seconds
	if (update_reg & SECONDS_UPDATED) {
		update_reg &=0xfe;
		w = second_forever;
		convert_w_to_decimal();
		round_text[0] = thousands;
		round_text[1] = tens;
		round_text[2] = ones;
		multi_vram_buffer_horz(round_text, sizeof(round_text), NTADR_A(9, 1));	
	}
	//clients
	if (update_reg & CLIENT_UPDATED) {
		update_reg &=0xfd;
		i = 14 - round;
		convert_i_to_decimal();
		round_text[0] = thousands;
		round_text[1] = tens;
		round_text[2] = ones;
		multi_vram_buffer_horz(round_text, sizeof(round_text), NTADR_A(11, 2));
	}
}

void print_challenge()
{
	// challenge
	i = 11;
	j = 5;
	
	if (challenge_table[challenge] == BLUE_CARD) {
		multi_vram_buffer_horz(tile_blue_top, sizeof(tile_blue_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_blue_down, sizeof(tile_blue_down), NTADR_A(i, j+1));
	} else if (challenge_table[challenge] == YELLOW_CARD) {
		multi_vram_buffer_horz(tile_yellow_top, sizeof(tile_yellow_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_yellow_down, sizeof(tile_yellow_down), NTADR_A(i, j+1));
	} else if (challenge_table[challenge] == RED_CARD) {
		multi_vram_buffer_horz(tile_red_top, sizeof(tile_red_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_red_down, sizeof(tile_red_down), NTADR_A(i, j+1));
	} else if (challenge_table[challenge] == GREEN_CARD) {
		multi_vram_buffer_horz(tile_green_top, sizeof(tile_green_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_green_down, sizeof(tile_green_down), NTADR_A(i, j+1));
	} else if (challenge_table[challenge] == BLACK_CARD){
		multi_vram_buffer_horz(tile_black_top, sizeof(tile_black_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_black_down, sizeof(tile_black_down), NTADR_A(i, j+1));
	}
	// prefered
	i = 17;
	j = 5;
	
	if (preferred_table[challenge] == BLUE_CARD) {
		multi_vram_buffer_horz(tile_blue_top, sizeof(tile_blue_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_blue_down, sizeof(tile_blue_down), NTADR_A(i, j+1));
	} else if (preferred_table[challenge] == YELLOW_CARD) {
		multi_vram_buffer_horz(tile_yellow_top, sizeof(tile_yellow_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_yellow_down, sizeof(tile_yellow_down), NTADR_A(i, j+1));
	} else if (preferred_table[challenge] == RED_CARD) {
		multi_vram_buffer_horz(tile_red_top, sizeof(tile_red_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_red_down, sizeof(tile_red_down), NTADR_A(i, j+1));
	} else if (preferred_table[challenge] == GREEN_CARD) {
		multi_vram_buffer_horz(tile_green_top, sizeof(tile_green_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_green_down, sizeof(tile_green_down), NTADR_A(i, j+1));
	} else if (preferred_table[challenge] == BLACK_CARD){
		multi_vram_buffer_horz(tile_black_top, sizeof(tile_black_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_black_down, sizeof(tile_black_down), NTADR_A(i, j+1));
	}

	// next
	i = 5;
	j = 5;
	n = (challenge+1) & 0x0f;

	if (challenge_table[n] == BLUE_CARD) {
		multi_vram_buffer_horz(tile_blue_top, sizeof(tile_blue_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_blue_down, sizeof(tile_blue_down), NTADR_A(i, j+1));
	} else if (challenge_table[n] == YELLOW_CARD) {
		multi_vram_buffer_horz(tile_yellow_top, sizeof(tile_yellow_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_yellow_down, sizeof(tile_yellow_down), NTADR_A(i, j+1));
	} else if (challenge_table[n] == RED_CARD) {
		multi_vram_buffer_horz(tile_red_top, sizeof(tile_red_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_red_down, sizeof(tile_red_down), NTADR_A(i, j+1));
	} else if (challenge_table[n] == GREEN_CARD) {
		multi_vram_buffer_horz(tile_green_top, sizeof(tile_green_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_green_down, sizeof(tile_green_down), NTADR_A(i, j+1));
	} else if (challenge_table[n] == BLACK_CARD){
		multi_vram_buffer_horz(tile_black_top, sizeof(tile_black_top), NTADR_A(i, j));
		multi_vram_buffer_horz(tile_black_down, sizeof(tile_black_down), NTADR_A(i, j+1));
	}
	
	// print client distant from table
	i = 8<<3;
	j = 10<<3;
	if (clients[n] == 0) {
		oam_meta_spr(i, j, metasprite_client0);
	}
	if (clients[n] == 1) {
		oam_meta_spr(i, j, metasprite_client1);
	}
	if (clients[n] == 2) {
		oam_meta_spr(i, j, metasprite_client2);
	}
	if (clients[n] == 3) {
		oam_meta_spr(i, j, metasprite_client3);
	}
	if (clients[n] == 4) {
		oam_meta_spr(i, j, metasprite_client4);
	}
	
	// print client close to table
	n = challenge;
	i = 14<<3;
	if (clients[n] == 0) {
		oam_meta_spr(i, j, metasprite_client0);
	}
	if (clients[n] == 1) {
		oam_meta_spr(i, j, metasprite_client1);
	}
	if (clients[n] == 2) {
		oam_meta_spr(i, j, metasprite_client2);
	}
	if (clients[n] == 3) {
		oam_meta_spr(i, j, metasprite_client3);
	}
	if (clients[n] == 4) {
		oam_meta_spr(i, j, metasprite_client4);
	}
}

void print_temp_card_on_pos_x_y(void)
{
	if (temp_card->value & 0xF0 && temp_card->value != 0) {
		oam_spr((i+1)<<3, (j+1)<<3, 45, 0);
		oam_spr((i+2)<<3, (j+1)<<3, ((0xff-temp_card->value)+1)+ZERO_CHAR, 0);
	} else {
		oam_spr((i+2)<<3, (j+1)<<3, temp_card->value+ZERO_CHAR, 0);
	}
	if (temp_card->Lmodifier != M_NONE) {
		oam_spr(i<<3, (j+4)<<3, 243, temp_card->Lmodifier);
	}
	if (temp_card->Rmodifier != M_NONE) {
		oam_spr((i+3)<<3, (j+4)<<3, 244, temp_card->Rmodifier);
	}
	if (temp_card->color == BLUE_CARD) {
		oam_meta_spr((i+1)<<3, (j+2)<<3, metasprite_blue);
	} else if (temp_card->color == YELLOW_CARD) {
		oam_meta_spr((i+1)<<3, (j+2)<<3, metasprite_yellow);
	} else if (temp_card->color == RED_CARD) {
		oam_meta_spr((i+1)<<3, (j+2)<<3, metasprite_red);
	} else if (temp_card->color == GREEN_CARD) {
		oam_meta_spr((i+1)<<3, (j+2)<<3, metasprite_green);
	} else if (temp_card->color == BLACK_CARD){
		oam_meta_spr((i+1)<<3, (j+2)<<3, metasprite_black);
	}
}

void print_cursor(void)
{
	if (cursor.cell < 4) {
		j = 19;
		i = 7 + cursor.cell*4;
	} else {
		j = 13;
		i = 7 + (cursor.cell - 4)*4;
	}
	oam_meta_spr(i<<3, j<<3, metasprite_cursor_up);
	oam_meta_spr(i<<3, (j+7)<<3, metasprite_cursor_down);
}

void update_cards_on_table(void)
{
	oam_clear();
	// print card cost
	i = 8, j=20;
	temp_card = &blue_cards[BLUE_IDX];
	print_temp_card_on_pos_x_y();
	i = 12;
	temp_card = &green_cards[GREEN_IDX];
	print_temp_card_on_pos_x_y();
	i = 16;
	temp_card = &yellow_cards[YELLOW_IDX];
	print_temp_card_on_pos_x_y();
	i = 20;
	temp_card = &red_cards[RED_IDX];
	print_temp_card_on_pos_x_y();
	
	if (cursor.card != NULL)
	{
		i = 24, j = 6;
		temp_card = cursor.card;
		print_temp_card_on_pos_x_y();
	}
	i = 8;
	j = 14;
	for (n = 0; n < 4; n++) {
		if ((*table_ptr)[n] != NULL)
		{
			temp_card = (*table_ptr)[n];
			print_temp_card_on_pos_x_y();
		}
		i += 4;
	}
	// print cursor position
	print_cursor();
	print_challenge();
}

void print_entry(void)
{
	if (map_registers != ENTRY_DRAWN) {
		load_room();
	}
}

void print_menu(void)
{
	if (map_registers != MENU_DRAWN) {
		load_room();
	}
}

void print_scores(void) 
{
	if (map_registers != END_SCREEN_DRAWN) {
		
		load_room();
		
		for (j = 0; j < 5; j++)
		{
			if (j == 0) {
				i = round_score;
			} else {
				i = best_scores[j];
			}

			convert_i_to_decimal();
			
			score_text[0] = 'L' + j;
			score_text[1] = 'S' + j;
			score_text[2] = 'C' + j;
			score_text[6] = thousands;
			score_text[7] = hundreds;
			score_text[8] = tens;
			score_text[9] = ones + j;
			
			multi_vram_buffer_horz(score_text, sizeof(score_text), NTADR_A(10, 8 + j*2));
		}
	}
}

void load_room() {
	// LOAD ROOM
	switch (GameState)
	{
	case GAME:
		set_data_pointer(room_bettergame);
		set_mt_pointer(room_bettergame_mt);
		map_registers = GAME_DRAWN;
		break;
	case ENDSCREEN:
		set_data_pointer(room_endscreen);
		set_mt_pointer(room_endscreen_mt);
		map_registers = END_SCREEN_DRAWN;
		break;
	case MENU:
		set_data_pointer(room_menu);
		set_mt_pointer(room_menu_mt);
		map_registers = ENTRY_DRAWN;
		break;
	case MENU_SETTINGS:
		set_data_pointer(room_settings);
		set_mt_pointer(room_settings_mt);
		map_registers = MENU_DRAWN;
		break;
	default:
		break;
	}
	ppu_off();
	oam_clear();
	for(i=0; ;i+=0x20){
		for(j=0; ;j+=0x20){
			clear_vram_buffer(); // do each frame, and before putting anything in the buffer
			address = get_ppu_addr(0, j, i);
			index = (i & 0xf0) + (j >> 4);
			buffer_4_mt(address, index); // ppu_address, index to the data
			flush_vram_update_nmi();
			if (j == 0xe0) break;
		}
		if (i == 0xe0) break;
	}
	// Paint the cards each with a different collor
	if (map_registers == GAME_DRAWN) {
		n = 0;
		for (i = 8; i < 8+13; i+=4) {
			address = get_at_addr(0, i<<3, 20<<3);
			vram_adr(address); // pick a random attribute byte on the lower half
			vram_put(card_palete[n]);
			address = get_at_addr(0, i<<3, 24<<3);
			vram_adr(address); // pick a random attribute byte on the lower half
			vram_put(card_palete[n]);
			n++;
		}
	}
	set_scroll_y(0xff);
	
	clear_vram_buffer(); 
	ppu_on_all();
}

void sleep(BYTE byte)
{
	if (byte > 60) {
		byte = 60;
	}
	desired = tick + byte;
    for (;desired > tick;) {
        _clock_counter();
        ppu_wait_nmi();
    }
}

void main(void)
{
	_init();
#ifdef UNITTEST
	run_tests();
#else
	_game_loop();
#endif
}

void _game_loop(void)
{
	while (1)
	{
		_clock_counter();
		_update60();
		_draw();
		_cleanup();
	}
}