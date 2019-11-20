/*	Panacea Pedler
 * 	@author scoppio
 *  @date 2019
 */

#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "lib/colornames.h"
#include "main.h"
#include "sprites.h"

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

unsigned char GameState = GAME;

#define SCREEN_POS(x, y) (vram_adr(NTADR_A(x, y)))
#define PRINT(t) (vram_write(t, sizeof(t)))
#define PRINT_AT(x, y, t) \
	SCREEN_POS(x, y);     \
	PRINT(t);             \
	SCREEN_POS(0, 0);

#define BTN(btn) (pad1_new & btn)
#define HEX_TO_CHAR(byt) (((byt & 0x0F) > 0x9) ? (byt & 0x0F) + SEVEN_CHAR : (byt & 0x0F) + ZERO_CHAR)
#define BLUE_IDX() (blue_idx[blue_size_pt])
#define GREEN_IDX() (green_idx[green_size_pt])
#define YELLOW_IDX() (yellow_idx[yellow_size_pt])
#define RED_IDX() (red_idx[red_size_pt])

void _init(void)
{
	ppu_off();			 // screen off
	pal_bg(palette_bg);  //	load the BG palette
	pal_spr(palette_sp); // load the Sprite palette
	ppu_on_all();		 //	turn on screen
	bank_spr(1);
	draw_bg();
}

void _update60(void)
{
	// do nothing atm;
	controller_update();
}

void shuffle_decks(void)
{
	shuffle(&red_idx);
	shuffle(&blue_idx);
	shuffle(&green_idx);
	shuffle(&yellow_idx);
}

void shuffle(unsigned char (*array)[13])
{
	for (i = 0; i < DECK_CARDS_SIZE - 1; i++)
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

void controller_update(void)
{
	pad_poll(0);			   // read the first controller
	pad1_new = get_pad_new(0); // newly pressed button. do pad_poll first

	if (GameState == GAME)
	{
		// to implement
		if (BTN(PAD_A))
		{
			// if the cell is under 4 it will place on the table
			// therefore it will place the card on the table at the cell it is targeting.
			switch (cursor.cell)
			{
			case 0:
				if (blue_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &blue_cards[BLUE_IDX()];
					blue_size_pt--;
				}
				break;
			case 1:
				if (yellow_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &yellow_cards[YELLOW_IDX()];
				}
				break;
			case 2:
				if (red_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &red_cards[RED_IDX()];
				}
				break;
			case 3:
				if (green_size_pt < DECK_CARDS_SIZE - 1)
				{
					cursor.card = &green_cards[GREEN_IDX()];
				}
				break;
			}
			if (!(cursor.cell & 0x4))
			{
				cursor.cell &= 0x4;
			}
		}
		if (BTN(PAD_B))
		{
			if ((cursor.card->id >> 4) == 0x1)
			{
				// red
				cursor.card = NULL;
				red_size_pt--;
			}
			else if ((cursor.card->id >> 4) == 0x2)
			{
				cursor.card = NULL;
				red_size_pt--;
			}
			else if ((cursor.card->id >> 4) == 0x3)
			{
				cursor.card = NULL;
				red_size_pt--;
			}
			else if ((cursor.card->id >> 4) == 0x4)
			{
				cursor.card = NULL;
				red_size_pt--;
			}
		}
		if (BTN(PAD_LEFT))
		{
			cursor.cell = cursor.cell == 0 ? 3 : cursor.cell - 1;
		}
		if (BTN(PAD_RIGHT))
		{
			cursor.cell = cursor.cell == 3 ? 0 : cursor.cell + 1;
		}
	}
	else if (GameState == MENU)
	{
		// not implemented yet
	}
}

void _draw(void)
{
	ppu_wait_nmi();
	// timer_draw();
	print_debug();
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
	PRINT_AT(10, 3, datetime);
}

void print_debug(void)
{
	cursor.card = &red_cards[red_idx[red_size_pt]];
	i = red_size_pt;
	convert_i_to_decimal();
	debug_text[7] = ones;
	debug_text[6] = tens;

	i = red_idx[red_size_pt];
	convert_i_to_decimal();
	debug_text[4] = ones;
	debug_text[3] = tens;

	i = cursor.card->id;
	debug_text[1] = HEX_TO_CHAR(i);
	debug_text[0] = HEX_TO_CHAR(i >> 4);

	PRINT_AT(10, 3, debug_text);
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

void _game_loop(void)
{
	while (1)
	{
		_clock_counter();
		_update60();
		_draw();
	}
}

void main(void)
{
	_init();
	_game_loop();
}
