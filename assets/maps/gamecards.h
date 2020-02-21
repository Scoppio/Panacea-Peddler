
const unsigned char challenge_table[]={
	3, 0, 1, 2, 3, 0, 1, 2, 3, 1, 0, 2, 3, 5, 0, 1
};

const unsigned char preferred_table[]={
	0, 1, 2, 3, 5, 1, 2, 3, 2, 0, 2, 3, 1, 0, 1, 3
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
	  0,  0,0xe6,0,
	  8,  0,0xf6,0,
	 16,  0,0xf6,0,
	 24,  0,0xf6,0,
	 32,  0,0xf6,0,
	 40,  0,0xe6,0,
	128
};

const unsigned char metasprite_cursor_down[]={
	  0,  0,0xe6,0,
	  8,  0,0xd6,0,
	 16,  0,0xd6,0,
	 24,  0,0xd6,0,
	 32,  0,0xd6,0,
	 40,  0,0xe6,0,
	128
};

const unsigned char card_palete[] ={
    0b01010101, // blue
    0b11111111, // green
    0b00000000, // yellow
    0b10101010, // red
};

const unsigned char cursor_mt[] = {
    230,214,214,214,214,230
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