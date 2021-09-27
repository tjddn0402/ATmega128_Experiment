#define F_CPU 1600000
#include <avr/io.h>
#include <util/delay.h>

void cmd(int command)
{
	PORTA = 0b00000000;//Enable=0
	_delay_ms(1);
	PORTA = 0b00000100;//Enable=1
	_delay_ms(1);
	PORTB = command;
	_delay_ms(1);
	PORTA = 0b00000000;
	_delay_ms(1);
}

void data(char str)
{
	PORTA = 0b00000101;
	_delay_ms(1);
	PORTB = str;
	_delay_ms(1);
	PORTA = 0b00000001;
	_delay_ms(1);
}

void display1(volatile char *str)
{
	cmd(0b10000000);
	int i = 0;
	while (str[i] != '\0')
	{
		data(str[i++]);
	}
}

volatile char alphabets[1][27] = {{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"}}; // Initial LCD display

int main()
{
	unsigned char cursor=0;
	DDRA = 0b11111111;
	DDRB = 0b11111111;

	_delay_ms(50);
	cmd(0b00110000); // Function Set. Set 8bit 1Line 5x7 dots
	cmd(0b00000001); // Display clear.
	cmd(0b10000000); // Set DDRAM address or cursor position on display
	cmd(0b00001100); // Display on. Cursor Off. Blink Off
	cmd(0b00000110); // Entry Mode. Cursor Increment

	display1(alphabets[0]);
	cmd(0b00000010);// Cursor Home
	cmd(0b00000101);// Cursor shift to right

	
	while (1)
	{
		if(cursor>=6){
			cursor=0;
			_delay_ms(10000);
			cmd(0b00000010);// Cursor Home
		}
		else{
			cursor++;
			_delay_ms(10000);
			cmd(00011000); // Display clear.

		}
	}
}