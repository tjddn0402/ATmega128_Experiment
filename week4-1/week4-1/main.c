#define F_CPU 1600000
#include <avr/io.h>
#include <util/delay.h>

void cmd(int command)
{
	PORTB = command;
	PORTA = 0x04;
	_delay_ms(1);
	PORTA = 0x00;
}
void data(char str)
{
	PORTB = str;
	PORTA = 0x05;
	_delay_ms(1);
	PORTA = 0x01;
}

void display1(volatile char *str)
{
	cmd(0b10000000);//
	int i = 0;
	while (str[i] != '\0')
	{
		data(str[i]);
		i++;
	}
}

volatile char name[1][16] = {{"Kim Seongwoo"}}; // Initial LCD display

int main()
{
	DDRA = 0b11111111;
	DDRB = 0b11111111;

	_delay_ms(50);
	cmd(0b00111000); // Function Set. Set 8bit 2Line 5x7 dots
	cmd(0b00000001); // Display clear.
	cmd(0b10000000); // Set DDRAM address or cursor position on display
	cmd(0b00001100); // Display on. Cursor Off. Blink Off
	cmd(0b00000110); // Entry Mode. Cursor Increment

	display1(name[0]);
	
	while (1)
	{	
	}
}