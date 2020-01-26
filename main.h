unsigned char pad1;
unsigned char pad1_new;
// static const unsigned char text[] = "Clock:"; // zero terminated c string
unsigned char datetime[] = "00:00:00";
unsigned char score_text[10] = "abc - 9999";
// static unsigned char debug_text[10] = "          ";

unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;
unsigned char counter = 60;
static const unsigned char SIXTY = 60;

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
typedef unsigned char BYTE;

struct SpObject
{
    unsigned char X;
    unsigned char Y;
    unsigned char width;
    unsigned char height;
};

#define BLUE_CARD 0
#define GREEN_CARD 1
#define YELLOW_CARD 2
#define RED_CARD 3
#define BLACK_CARD 5
#define DECK_CARDS_SIZE 13

#define M_RANDOM 4
#define M_NONE 12

struct Card
{
    unsigned char id;
    signed char value;
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

struct Card *table[4];

unsigned char cards_size_ptr[4] = {0,0,0,0};

static signed int best_scores[5] = {0, 0, 0, 0, 0};
signed int round_score = 0;
unsigned char round = 0;

unsigned char red_bc_count = 3;
unsigned char yellow_bc_count = 3;
unsigned char green_bc_count = 3;
unsigned char blue_bc_count = 3;

unsigned char red_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
unsigned char yellow_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
unsigned char green_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
unsigned char blue_idx[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

/*

 *  0 = blue
 *  1 = green
 *  2 = yellow
 *  3 = red
 * */

struct Card blue_cards[] = {
    {0x00, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x01, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x02, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x03, 1, BLUE_CARD, M_RANDOM, M_NONE},
    {0x04, 1, BLUE_CARD, M_RANDOM, M_NONE},
    {0x05, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x06, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x07, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x08, 1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {0x09, 1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {0x0A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x0B, -3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x0C, -1, BLACK_CARD, M_RANDOM, M_RANDOM}};

struct Card green_cards[] = {
    {0x10, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x11, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x12, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x13, 1, GREEN_CARD, M_RANDOM, M_NONE},
    {0x14, 1, GREEN_CARD, M_RANDOM, M_NONE},
    {0x15, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x16, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x17, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x18, 1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {0x19, 1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {0x1A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x1B, -3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x1C, -1, BLACK_CARD, M_RANDOM, M_RANDOM}};

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
    {0x2A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x2B, -3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x2C, -1, BLACK_CARD, M_RANDOM, M_RANDOM}};

struct Card red_cards[] = {
    {0x30, 1, RED_CARD, M_NONE, M_NONE},
    {0x31, 1, RED_CARD, M_NONE, M_NONE},
    {0x32, 1, RED_CARD, M_NONE, M_NONE},
    {0x33, 1, RED_CARD, M_RANDOM, M_NONE},
    {0x34, 1, RED_CARD, M_RANDOM, M_NONE},
    {0x35, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x36, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x37, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x38, 1, RED_CARD, M_RANDOM, M_RANDOM},
    {0x39, 1, RED_CARD, M_RANDOM, M_RANDOM},
    {0x3A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x3B, -3, BLACK_CARD, M_NONE, M_RANDOM},
    {0x3C, -1, BLACK_CARD, M_RANDOM, M_RANDOM}};

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
void main(void);
void _game_loop(void);
void _init(void);
void _draw(void);
void _update60(void);
void reset_game(void);
void draw_sprites(void);
void draw_bg(void);
void timer_draw(void);
void print_debug(void);
// void controller_update(void);
void controller(void);
void controller_menu(void);
void controller_menu_settings(void);
void controller_game(void);
void controller_endscreen(void);
void shuffle_decks(void);
void instantiate_card_modifiers(struct Card * cards);
void shuffle(unsigned char (*array)[13]);
void interact_with_table(void);
void cancel_card(void);
void end_of_round(void);
int count_points(void);