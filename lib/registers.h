
unsigned char i = 0;
unsigned char n = 0;
unsigned char j = 0;
unsigned char t = 0;
unsigned int w = 0;

unsigned char ones = 0;
unsigned char tens = 0;
unsigned char hundreds = 0;
unsigned char thousands = 0;

static const unsigned char ZERO_CHAR = '0';

void convert_i_to_decimal(void)
{
    ones = 0;
    tens = 0;
    hundreds = 0;
    thousands = 0;
    while (i)
    {
        if (i > 99)
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

void convert_w_to_decimal(void)
{
    ones = 0;
    tens = 0;
    hundreds = 0;
    thousands = 0;
    while (w)
    {
        
        if (w > 999)
        {
            w -= 1000;
            thousands++;
        }
        else if (w > 99)
        {
            i -= 100;
            hundreds++;
        }
        else if (w > 9)
        {
            w -= 10;
            tens++;
        }
        else
        {
            ones = w;
            w = 0;
        }
    }
    ones += ZERO_CHAR;
    tens += ZERO_CHAR;
    hundreds += ZERO_CHAR;
    thousands += ZERO_CHAR;
}
