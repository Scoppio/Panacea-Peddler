
unsigned char i = 0;
unsigned char n = 0;
unsigned char j = 0;
unsigned char t = 0;

static unsigned char ones = 0;
static unsigned char tens = 0;
static unsigned char hundreds = 0;
static unsigned char thousands = 0;

static const unsigned char ZERO_CHAR = '0';

void convert_i_to_decimal(void)
{
    ones = 0;
    tens = 0;
    hundreds = 0;
    thousands = 0;
    while (i)
    {
        
        if (i > 999)
        {
            i -= 1000;
            thousands++;
        }
        else if (i > 99)
        {
            i -= 100;
            hundreds++;
        }
        else if (i > 9)
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
    hundreds += ZERO_CHAR;
    thousands += ZERO_CHAR;
}
