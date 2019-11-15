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

unsigned char GameState = MENU;

#define SCREEN_POS(x, y) (vram_adr(NTADR_A(x, y)))
#define PRINT(t) (vram_write(t, sizeof(t)))
#define PRINT_AT(x, y, t) \
	SCREEN_POS(x, y);     \
	PRINT(t);             \
	SCREEN_POS(0, 0);

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
}

void _draw(void)
{
	ppu_wait_nmi();
	// clear_vram_buffer();
	timer_draw();
}

void timer_draw(void)
{
	i = second;
	_count_one_and_tens();
	datetime[7] = ones + '0';
	datetime[6] = tens + '0';
	i = minute;
	_count_one_and_tens();
	datetime[4] = ones + '0';
	datetime[3] = tens + '0';
	i = hour;
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
