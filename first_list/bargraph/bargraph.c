#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
const int light_time = 120;

int main()
{
	UCSR0B  &= ~_BV(RXEN0) & ~_BV(TXEN0);

    PORTD = 0x00;
    DDRD = 0xff;
	LED_PORT |= 1<<0;
    while(1)
    {
        for(int i = 0;i < 8;i++)
        {
            LED_PORT |= 1<<i;
            _delay_ms(light_time);
            LED_PORT &= ~(1<<i);
        }
	for (int i = 6; i >= 1; i--)
	{
            LED_PORT |= 1<<i;
	    _delay_ms(light_time);
	    LED_PORT &= ~(1<<i);
	}
    }

}
