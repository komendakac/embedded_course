#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB


const int delay_time = 500;

// inicjalizacja UART 
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}


void delay(int c){
	for (int i = 0; i < c; i++){
		_delay_ms(delay_time);
	}
}


void display_space(int multplier){
  LED_PORT &= ~_BV(LED);
  delay(multplier);
}


void show_long_signal(){
  LED_PORT |= _BV(LED);
  _delay_ms(delay_time * 3);
  LED_PORT &= ~_BV(LED);
  _delay_ms(delay_time);
}  


void display_character(char c){
	int k = c - 'A';
	int morse_codes[26][4] = 
	{
		{1, 3},
		{3, 1, 1, 1},
		{3, 1, 3, 1}, 
		{3, 1, 1},
		{1},
		{1, 1, 3, 1},
		{3, 3, 1},
		{1, 1, 1, 1}, 
		{1, 1},
		{1, 3, 3, 3},
		{3, 1, 3},
		{1, 3, 1, 1},
		{3, 3},
		{3, 1},
		{3, 3, 3},
		{1, 3, 3, 1},
		{3, 3, 1, 3},
		{1, 3, 1},
		{1, 1, 1},
		{3},
		{1, 1, 3},
		{1, 1, 1, 3},
		{1, 3, 3},
		{3, 1, 1, 3},
		{3, 1, 3, 3},
		{3, 3, 1, 1}
	};
	int index = 0;
	while (index < 4 && morse_codes[k][index]){
		LED_PORT |= _BV(LED);
		delay(morse_codes[k][index]);
		display_space(1);
		index++;
	}
}



FILE uart_file;

int main()
{
  LED_DDR |= _BV(LED);	
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  while(1) {
    char c = 1;
    c = (int)getchar();

    if (c <= 'Z' && c >= 'A'){
    	display_character(c);
    	display_space(2);
    }
    else{
		display_space(4);
    }
  }
}

