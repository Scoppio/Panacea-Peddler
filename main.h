
unsigned char i = 0;
static unsigned char ones = 0;
static unsigned char tens = 0;
static unsigned char second = 0;
static unsigned char minute = 0;
static unsigned char hour = 0;
static unsigned char counter = 60;
static const unsigned char SIXTY = 60;
static const unsigned char text[] = "Clock:"; // zero terminated c string
static unsigned char datetime[] = "00:00:00";

const unsigned char palette_bg[] = {
    0x0f, 0x00, 0x10, 0x30,
    0x0f, 0x01, 0x21, 0x39,
    0x0f, 0x04, 0x24, 0x36,
    0x0f, 0x09, 0x29, 0x38};

const unsigned char palette_sp[] = {
    0x0f, 0x00, 0x10, 0x30, // greys
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0};

// PROTOTYPES
void _draw(void);
void _update60(void);
void game_loop(void);
void draw_sprites(void);
void timer_draw(void);
void draw_bg(void);

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
