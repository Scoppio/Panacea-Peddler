
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
    unsigned char value;
    unsigned char color;
    unsigned char Lmodifier;
    unsigned char Rmodifier;
};

struct Cursor
{
    struct Card *cardId;
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

struct Card red_cards[] = {
    {1, RED_CARD, M_NONE, M_NONE},
    {1, RED_CARD, M_NONE, M_NONE},
    {1, RED_CARD, M_NONE, M_NONE},
    {1, RED_CARD, M_RANDOM, M_NONE},
    {1, RED_CARD, M_RANDOM, M_NONE},
    {1, RED_CARD, M_NONE, M_RANDOM},
    {1, RED_CARD, M_NONE, M_RANDOM},
    {1, RED_CARD, M_NONE, M_RANDOM},
    {1, RED_CARD, M_RANDOM, M_RANDOM},
    {1, RED_CARD, M_RANDOM, M_RANDOM},
    {2, BLACK_CARD, M_RANDOM, M_NONE},    // 10
    {3, BLACK_CARD, M_NONE, M_RANDOM},    // 11
    {1, BLACK_CARD, M_RANDOM, M_RANDOM}}; // 12

struct Card yellow_cards[] = {
    {1, YELLOW_CARD, M_NONE, M_NONE},
    {1, YELLOW_CARD, M_NONE, M_NONE},
    {1, YELLOW_CARD, M_NONE, M_NONE},
    {1, YELLOW_CARD, M_RANDOM, M_NONE},
    {1, YELLOW_CARD, M_RANDOM, M_NONE},
    {1, YELLOW_CARD, M_NONE, M_RANDOM},
    {1, YELLOW_CARD, M_NONE, M_RANDOM},
    {1, YELLOW_CARD, M_NONE, M_RANDOM},
    {1, YELLOW_CARD, M_RANDOM, M_RANDOM},
    {1, YELLOW_CARD, M_RANDOM, M_RANDOM},
    {2, BLACK_CARD, M_RANDOM, M_NONE},
    {3, BLACK_CARD, M_NONE, M_RANDOM},
    {1, BLACK_CARD, M_RANDOM, M_RANDOM}};

struct Card green_cards[] = {
    {1, GREEN_CARD, M_NONE, M_NONE},
    {1, GREEN_CARD, M_NONE, M_NONE},
    {1, GREEN_CARD, M_NONE, M_NONE},
    {1, GREEN_CARD, M_RANDOM, M_NONE},
    {1, GREEN_CARD, M_RANDOM, M_NONE},
    {1, GREEN_CARD, M_NONE, M_RANDOM},
    {1, GREEN_CARD, M_NONE, M_RANDOM},
    {1, GREEN_CARD, M_NONE, M_RANDOM},
    {1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {2, BLACK_CARD, M_RANDOM, M_NONE},
    {3, BLACK_CARD, M_NONE, M_RANDOM},
    {1, BLACK_CARD, M_RANDOM, M_RANDOM}};

struct Card blue_cards[] = {
    {1, BLUE_CARD, M_NONE, M_NONE},
    {1, BLUE_CARD, M_NONE, M_NONE},
    {1, BLUE_CARD, M_NONE, M_NONE},
    {1, BLUE_CARD, M_RANDOM, M_NONE},
    {1, BLUE_CARD, M_RANDOM, M_NONE},
    {1, BLUE_CARD, M_NONE, M_RANDOM},
    {1, BLUE_CARD, M_NONE, M_RANDOM},
    {1, BLUE_CARD, M_NONE, M_RANDOM},
    {1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {2, BLACK_CARD, M_RANDOM, M_NONE},
    {3, BLACK_CARD, M_NONE, M_RANDOM},
    {1, BLACK_CARD, M_RANDOM, M_RANDOM}};

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
void timer_draw(void);
void print_debug(void);
void draw_bg(void);
void controller_update(void);
void shuffle_decks(void);
// void draw_cards_table(void);
// void draw_card_piles(void);

void _count_one_and_tens(void)
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
