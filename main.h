
unsigned char pad1;
unsigned char pad1_new;
unsigned char i = 0;
unsigned char n = 0;
unsigned char j = 0;
unsigned char t = 0;
static unsigned char ones = 0;
static unsigned char tens = 0;
static unsigned char second = 0;
static unsigned char minute = 0;
static unsigned char hour = 0;
static unsigned char counter = 60;
static const unsigned char SIXTY = 60;
static const unsigned char text[] = "Clock:"; // zero terminated c string
static unsigned char datetime[] = "00:00:00";
static unsigned char debug_text[10] = "          ";

typedef unsigned char BYTE;

struct SpObject
{
    unsigned char X;
    unsigned char Y;
    unsigned char width;
    unsigned char height;
};

#define BLUE_CARD 0
#define RED_CARD 1
#define YELLOW_CARD 2
#define GREEN_CARD 3
#define BLACK_CARD 4
#define DECK_CARDS_SIZE 13

#define M_NONE 0
#define M_YELLOW 1
#define M_RED 2
#define M_BLUE 3
#define M_BLACK 4
#define M_RANDOM 5

struct Card
{
    unsigned char id;
    unsigned char value;
    unsigned char color;
    unsigned char Lmodifier;
    unsigned char Rmodifier;
};

struct Cursor
{
    struct Card *card;
    unsigned char cell;
} cursor;

// struct SpObject Paddle = {0x75, PADDLE_Y, 0x1f, 7};
// struct SpObject Ball = {0xff, 0xff, 5, 5}; // balls x and x will be init later

const static unsigned char red_size = 13;
const static unsigned char yellow_size = 13;
const static unsigned char green_size = 13;
const static unsigned char blue_size = 13;

static unsigned char red_size_pt = 0;
static unsigned char yellow_size_pt = 0;
static unsigned char green_size_pt = 0;
static unsigned char blue_size_pt = 0;

static unsigned char red_bc_count = 3;
static unsigned char yellow_bc_count = 3;
static unsigned char green_bc_count = 3;
static unsigned char blue_bc_count = 3;

static unsigned char red_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
static unsigned char yellow_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
static unsigned char green_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
static unsigned char blue_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

static const unsigned char ZERO_CHAR = '0';
static const unsigned char SEVEN_CHAR = '7';

struct Card red_cards[] = {
    {0x10, 1, RED_CARD, M_NONE, M_NONE},
    {0x11, 1, RED_CARD, M_NONE, M_NONE},
    {0x12, 1, RED_CARD, M_NONE, M_NONE},
    {0x13, 1, RED_CARD, M_RANDOM, M_NONE},
    {0x14, 1, RED_CARD, M_RANDOM, M_NONE},
    {0x15, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x16, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x17, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x18, 1, RED_CARD, M_RANDOM, M_RANDOM},
    {0x19, 1, RED_CARD, M_RANDOM, M_RANDOM},
    {0x1A, 2, BLACK_CARD, M_RANDOM, M_NONE},    // 10
    {0x1B, 3, BLACK_CARD, M_NONE, M_RANDOM},    // 11
    {0x1C, 1, BLACK_CARD, M_RANDOM, M_RANDOM}}; // 12

struct Card yellow_cards[] = {
    {0x20, 1, YELLOW_CARD, M_NONE, M_NONE},
    {0x21, 1, YELLOW_CARD, M_NONE, M_NONE},
    {0x22, 1, YELLOW_CARD, M_NONE, M_NONE},
    {0x23, 1, YELLOW_CARD, M_RANDOM, M_NONE},
    {0x24, 1, YELLOW_CARD, M_RANDOM, M_NONE},
    {0x25, 1, YELLOW_CARD, M_NONE, M_RANDOM},
    {0x26, 1, YELLOW_CARD, M_NONE, M_RANDOM},
    {0x27, 1, YELLOW_CARD, M_NONE, M_RANDOM},
    {0x28, 1, YELLOW_CARD, M_RANDOM, M_RANDOM},
    {0x29, 1, YELLOW_CARD, M_RANDOM, M_RANDOM},
    {0x2A, 2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x2B, 3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x2C, 1, BLACK_CARD, M_RANDOM, M_RANDOM}};

struct Card green_cards[] = {
    {0x30, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x31, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x32, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x33, 1, GREEN_CARD, M_RANDOM, M_NONE},
    {0x34, 1, GREEN_CARD, M_RANDOM, M_NONE},
    {0x35, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x36, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x37, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x38, 1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {0x39, 1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {0x3A, 2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x3B, 3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x3C, 1, BLACK_CARD, M_RANDOM, M_RANDOM}};

struct Card blue_cards[] = {
    {0x40, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x41, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x42, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x43, 1, BLUE_CARD, M_RANDOM, M_NONE},
    {0x44, 1, BLUE_CARD, M_RANDOM, M_NONE},
    {0x45, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x46, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x47, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x48, 1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {0x49, 1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {0x4A, 2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x4B, 3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x4C, 1, BLACK_CARD, M_RANDOM, M_RANDOM}};

const unsigned char palette_bg[] = {
    BLACK, DARK_GREY, SILVER, WHITE,
    0x0f, 0x01, 0x21, 0x39,
    0x0f, 0x04, 0x24, 0x36,
    0x0f, 0x09, 0x29, 0x38};

const unsigned char palette_bg2[] = {
    BLUE, OCHRE, YELLOW, RED,
    0x0f, 0x01, 0x21, 0x39,
    0x0f, 0x04, 0x24, 0x36,
    0x0f, 0x09, 0x29, 0x38};

const unsigned char palette_sp[] = {
    BLACK, DARK_GREY, SILVER, WHITE,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0};

// PROTOTYPES
void _draw(void);
void _update60(void);
void game_loop(void);
void draw_sprites(void);
void draw_bg(void);
void timer_draw(void);
void print_debug(void);
void controller_update(void);
void shuffle_decks(void);
void shuffle(unsigned char (*array)[13]);

void convert_i_to_decimal(void)
{
    ones = 0;
    tens = 0;
    while (i)
    {
        if (i > 9)
        {
            i -= 10;
            tens++;
        }
        else
        {
            ones = i;
            i = 0;
        }
    }
    ones += ZERO_CHAR;
    tens += ZERO_CHAR;
}

void _clock_counter(void)
{
    counter--;
    if (counter == 0)
    {
        second++;
        counter = SIXTY;
        if (second == SIXTY)
        {
            minute++;
            second = 0;
            if (minute == SIXTY)
            {
                hour++;
                minute = 0;
            }
        }
    }
}
