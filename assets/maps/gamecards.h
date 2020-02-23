
const unsigned char clients[]={
	0, 1, 2, 4, 3, 1, 0, 3, 2, 4, 1, 0, 1, 3, 2, 4
};

const unsigned char challenge_table[]={
	3, 0, 1, 2, 3, 0, 1, 2, 3, 1, 0, 2, 3, 5, 0, 1
};

const unsigned char preferred_table[]={
	0, 1, 2, 3, 5, 1, 2, 3, 2, 0, 2, 3, 1, 0, 1, 3
};

const unsigned char tile_yellow_top[]={
	176, 177
};
const unsigned char tile_yellow_down[]={
	192, 193
};


const unsigned char tile_blue_top[]={
	178, 179
};
const unsigned char tile_blue_down[]={
	194, 195
};


const unsigned char tile_red_top[]={
	180, 181
};
const unsigned char tile_red_down[]={
	196, 197
};


const unsigned char tile_green_top[]={
	182, 183
};
const unsigned char tile_green_down[]={
	198, 199
};


const unsigned char tile_black_top[]={
	102, 103
};
const unsigned char tile_black_down[]={
	118, 119
};
const unsigned char metasprite_yellow[]={
	  0,  0,0xb0,0,
	  8,  0,0xb1,0,
	  0,  8,0xc0,0,
	  8,  8,0xc1,0,
	128
};

const unsigned char metasprite_blue[]={
	  0,  0,0xb2,0,
	  8,  0,0xb3,0,
	  0,  8,0xc2,0,
	  8,  8,0xc3,0,
	128
};

const unsigned char metasprite_red[]={
	  0,  0,0xb4,0,
	  8,  0,0xb5,0,
	  0,  8,0xc4,0,
	  8,  8,0xc5,0,
	128
};

const unsigned char metasprite_green[]={
	  0,  0,0xb6,0,
	  8,  0,0xb7,0,
	  0,  8,0xc6,0,
	  8,  8,0xc7,0,
	128
};

const unsigned char metasprite_black[]={
	  0,  0,0x66,0,
	  8,  0,0x67,0,
	  0,  8,0x76,0,
	  8,  8,0x77,0,
	128
};

const unsigned char metasprite_cursor_up[]={
	  4,  3,0xd5,0,
	 36,  3,0xd7,0,
	128
};

const unsigned char metasprite_cursor_down[]={
	  4,  -3,0xf5,0,
	 36,  -3,0xf7,0,
	128
};

const unsigned char metasprite_client0[]={
	  0,  0,0x80, 0,
	  8,  0,0x81, 0,
	  0,  8,0x90, 0,
	  8,  8,0x91, 0,
	  0,  16,0xa0,0,
	  8,  16,0xa1,0,
	128
};

const unsigned char metasprite_client1[]={
	  0,  0,0x86, 0,
	  8,  0,0x87, 0,
	  0,  8,0x96, 0,
	  8,  8,0x97, 0,
	  0,  16,0xa6,0,
	  8,  16,0xa7,0,
	128
};

const unsigned char metasprite_client2[]={
	  0,  0,0x84, 0,
	  8,  0,0x85, 0,
	  0,  8,0x94, 0,
	  8,  8,0x95, 0,
	  0,  16,0xa4,0,
	  8,  16,0xa5,0,
	128
};
const unsigned char metasprite_client3[]={
	  0,  0,0x82, 0,
	  8,  0,0x83, 0,
	  0,  8,0x92, 0,
	  8,  8,0x93, 0,
	  0,  16,0xa2,0,
	  8,  16,0xa3,0,
	128
};
const unsigned char metasprite_client4[]={
	  0,  0,0x80, 0,
	  8,  0,0x81, 0,
	  0,  8,0x90, 0,
	  8,  8,0x91, 0,
	  0,  16,0xa0,0,
	  8,  16,0xa1,0,
	128
};

const unsigned char card_palete[] ={
    0b01010101, // blue
    0b11111111, // green
    0b00000000, // yellow
    0b10101010, // red
};

// unused
const unsigned char card[]={
    208,209,
    224,225,

    209,210,
    225,226,
    
    224,225,
    224,225,

    225,226,
    225,226,
    
    224,225,
    240,241,

    225,226,
    241,242
};