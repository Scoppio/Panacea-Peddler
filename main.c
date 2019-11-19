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

void _init(void)
{
	ppu_off();			 // screen off
	pal_bg(palette_bg);  //	load the BG palette
	pal_spr(palette_sp); // load the Sprite palette
	ppu_on_all();		 //	turn on screen
	bank_spr(1);
	draw_bg();
	shuffle_decks();
}

void _update60(void)
{
	// do nothing atm;
	controller_update();
}

void shuffle_decks(void)
{

	for (i = 0; i < DECK_CARDS_SIZE - 1; i++)
	{
		do
		{
			j = rand8() & 0xF + i;
		} while (j >= DECK_CARDS_SIZE);
		t = red_idx[j];
		red_idx[j] = red_idx[i];
		red_idx[i] = t;
	}

	for (i = 0; i < DECK_CARDS_SIZE - 1; i++)
	{
		do
		{
			j = rand8() & 0xF + i;
		} while (j >= DECK_CARDS_SIZE);
		t = yellow_idx[j];
		yellow_idx[j] = yellow_idx[i];
		yellow_idx[i] = t;
	}

	for (i = 0; i < DECK_CARDS_SIZE - 1; i++)
	{
		do
		{
			j = rand8() & 0xF + i;
		} while (j >= DECK_CARDS_SIZE);
		t = green_idx[j];
		green_idx[j] = green_idx[i];
		green_idx[i] = t;
	}

	for (i = 0; i < DECK_CARDS_SIZE - 1; i++)
	{
		do
		{
			j = rand8() & 0xF + i;
		} while (j >= DECK_CARDS_SIZE);
		t = blue_idx[j];
		blue_idx[j] = blue_idx[i];
		blue_idx[i] = t;
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
			// switch (cursor.cell)
			// {
			// case 0:
			// 	cursor.cardId = &blue_cards[0];
			// 	break;
			// case 1:
			// 	cursor.cardId = &yellow_cards[0];
			// 	break;
			// case 2:
			// 	cursor.cardId = &red_cards[0];
			// 	break;
			// case 3:
			// 	cursor.cardId = &green_cards[0];
			// 	break;
			// }
			// if (!(cursor.cell & 0x4))
			// {
			// 	cursor.cell &= 0x4;
			// }
		}
		if (BTN(PAD_B))
		{
			if (red_size_pt < DECK_CARDS_SIZE - 1)
			{
				red_size_pt++;
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
	timer_draw();

	//draw_card_piles();
	//draw_cards_table();
}

void timer_draw(void)
{
	i = second;
	_count_one_and_tens();
	datetime[7] = ones + '0';
	datetime[6] = tens + '0';
	i = minute;
	i = red_size_pt;
	_count_one_and_tens();
	datetime[4] = ones + '0';
	datetime[3] = tens + '0';
	i = hour;
	i = red_idx[red_size_pt];
	_count_one_and_tens();
	datetime[1] = ones + '0';
	datetime[0] = tens + '0';
	PRINT_AT(10, 3, datetime);
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
