unsigned char pad1;
unsigned char pad1_new;

// Texts
unsigned char datetime[] = "00:00:00";

const unsigned char ENTRY_TEXT [] = "Entry ";
const unsigned char MENU_TEXT [] = "MENU  ";
const unsigned char GAME_TEXT [] = "GAME  ";
const unsigned char SCORE_TEXT [] = "SCORE:";
unsigned char score_text[] = "ABC - 9999";

unsigned char cards_count [] = "00  00  00  00";
unsigned char table_debug_text [] = " 00 00 00 00";
unsigned char deck_debug_text [] = " 00 00 00 00";
unsigned char cursor_text [] = "C: 0 C-ID: 00";
unsigned char round_text [] = "000";

unsigned char SLEEP = FALSE;
unsigned int desired = 0;
unsigned int tick = 0;
unsigned char second = 0;
unsigned int second_forever = 0;
unsigned char minute = 0;
unsigned char hour = 0;
unsigned char counter = 20;
static const unsigned char SIXTY = 60;

typedef unsigned char COLOR_;
typedef unsigned char BYTE;

#define BLUE_CARD 0
#define GREEN_CARD 1
#define YELLOW_CARD 2
#define RED_CARD 3

#define BLACK_CARD 5
#define DECK_CARDS_SIZE 13

#define M_RANDOM 4
#define M_NONE 12

#define ENTRY_DRAWN 1
#define END_SCREEN_DRAWN 2
#define MENU_DRAWN 4
#define GAME_DRAWN 8

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
};

struct Cursor cursor;
struct Card *table[4];

struct Card *(*table_ptr)[4] = &table;

unsigned char cards_size_ptr[4] = {0,0,0,0};
unsigned char placed_on_table = NULL;
static signed int best_scores[5] = {0, 0, 0, 0, 0};
signed int round_score = 0;
unsigned char round = 0;
signed char pp = 0;

unsigned char challenge = 0;
unsigned char preferred = 0;

unsigned char map_registers = 0x00;

unsigned char red_bc_count = 3;
unsigned char yellow_bc_count = 3;
unsigned char green_bc_count = 3;
unsigned char blue_bc_count = 3;

unsigned char cheat_num = 0;

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
    {0x0E, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x01, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x0B, -1, BLACK_CARD, M_NONE, M_RANDOM},
    {0x02, 1, BLUE_CARD, M_NONE, M_NONE},
    {0x03, 1, BLUE_CARD, M_RANDOM, M_NONE},
    {0x04, 1, BLUE_CARD, M_RANDOM, M_NONE},
    {0x05, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x0A, -2, BLACK_CARD, RED_CARD, M_NONE},
    {0x06, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x07, 1, BLUE_CARD, M_NONE, M_RANDOM},
    {0x08, 1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {0x09, 1, BLUE_CARD, M_RANDOM, M_RANDOM},
    {0x0C, -1, BLACK_CARD, M_NONE, M_NONE}};

struct Card green_cards[] = {
    {0x1E, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x1B, -1, BLACK_CARD, M_NONE, M_RANDOM},
    {0x11, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x12, 1, GREEN_CARD, M_NONE, M_NONE},
    {0x13, 1, GREEN_CARD, M_RANDOM, M_NONE},
    {0x14, 1, GREEN_CARD, M_RANDOM, M_NONE},
    {0x15, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x16, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x1A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x17, 1, GREEN_CARD, M_NONE, M_RANDOM},
    {0x18, 1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {0x19, 1, GREEN_CARD, M_RANDOM, M_RANDOM},
    {0x1C, -1, BLACK_CARD, M_NONE, M_NONE}
};

struct Card yellow_cards[] = {
    {0x2E, 1, YELLOW_CARD, M_NONE, M_NONE},
    {0x2B, -1, BLACK_CARD, M_NONE, M_RANDOM},
    {0x21, 1, YELLOW_CARD, M_NONE, M_NONE},
    {0x22, 1, YELLOW_CARD, M_NONE, M_NONE},
    {0x23, 1, YELLOW_CARD, M_RANDOM, M_NONE},
    {0x24, 1, YELLOW_CARD, M_RANDOM, M_NONE},
    {0x2A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x25, 1, YELLOW_CARD, M_NONE, M_RANDOM},
    {0x26, 1, YELLOW_CARD, M_NONE, M_RANDOM},
    {0x27, 1, YELLOW_CARD, M_NONE, M_RANDOM},
    {0x28, 1, YELLOW_CARD, M_RANDOM, M_RANDOM},
    {0x29, 1, YELLOW_CARD, M_RANDOM, M_RANDOM},
    {0x2C, -1, BLACK_CARD, M_NONE, M_NONE}};

struct Card red_cards[] = {
    {0x3E, 1, RED_CARD, M_NONE, M_NONE},
    {0x31, 1, RED_CARD, M_NONE, M_NONE},
    {0x3A, -2, BLACK_CARD, M_RANDOM, M_NONE},
    {0x32, 1, RED_CARD, M_NONE, M_NONE},
    {0x33, 1, RED_CARD, M_RANDOM, M_NONE},
    {0x34, 1, RED_CARD, M_RANDOM, M_NONE},
    {0x35, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x36, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x37, 1, RED_CARD, M_NONE, M_RANDOM},
    {0x3B, -1, BLACK_CARD, M_NONE, M_RANDOM},
    {0x38, 1, RED_CARD, M_RANDOM, M_RANDOM},
    {0x39, 1, RED_CARD, M_RANDOM, M_RANDOM},
    {0x3C, -1, BLACK_CARD, M_NONE, M_NONE}};

struct Card * temp_card;

const unsigned char palette_bg[] = {
    0x0f, 0x28, 0x17, 0x3c,
    0x0f, 0x1c, 0x28, 0x39,
    0x0f, 0x05, 0x15, 0x26,
    0x0f, 0x09, 0x1a, 0x2a};

const unsigned char palette_sp[] = {
    0x0f, 0x17, 0x28, 0x3c,
    0x0f, 0x1c, 0x28, 0x39,
    0x0f, 0x05, 0x15, 0x26,
    0x0f, 0x09, 0x1a, 0x2a};
    
// PROTOTYPES
void main(void);
void _game_loop(void);
void _init(void);
void _draw(void);
void _update60(void);
void _cleanup(void);

void reset_game(void);

void timer_draw(void);

void load_room(void);
void print_entry(void);
void print_menu(void);
void print_table(void);
void print_scores(void);
void print_temp_card_on_pos_x_y(void);
void print_cursor(void);
void print_challenge(void);

void update_card_count(void);
void update_score_header(void);
void update_cards_on_table(void);

void get_card_on_deck(void);

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
signed char count_points(void);

void sleep(BYTE byte);


void _clock_counter(void)
{
    tick++;
    counter--;
    
    if (counter == 0)
    {
        second++;
        second_forever++;
        counter = 20;
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
