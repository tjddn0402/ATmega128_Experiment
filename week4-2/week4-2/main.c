#define F_CPU 1600000
#include <avr/io.h>
#include <util/delay.h>

void cmd(int command)
{
	PORTB = command;
	PORTA = 0x04; //Enable=1, RS=0(명령 레지스터 선택)
	_delay_ms(1);
	PORTA = 0x00; //Enable=0
}
void data(char str)
{
	PORTB = str;
	PORTA = 0x05; //Enable=1, RW=0(쓰기), RS=1(데이터 레지스터 선택)
	_delay_ms(1);
	PORTA = 0x01; //Enable=0
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
	unsigned char cursor = 0;
	DDRA = 0b11111111;
	DDRB = 0b11111111;

	_delay_ms(50);
	cmd(0b00110000); // Function Set. Set 8bit 1Line 5x7 dots
	cmd(0b00000001); // Display clear.
	cmd(0b10000000); // Set DDRAM address or cursor position on display
	cmd(0b00001100); // Display on. Cursor Off. Blink Off
	cmd(0b00000110); // Entry Mode. Cursor Increment

	display1(alphabets[0]);
	cmd(0b00000010); // Cursor Home
	cmd(0b00000101); // Cursor shift to right

	while (1)
	{
		if (cursor >= 10)
		{
			cursor = 0;
			_delay_ms(1000);
			cmd(0b00000010); // Cursor Home
		}
		else
		{
			cursor++;
			_delay_ms(1000);
			cmd(0b00011000); // Display shift left.
		}
	}
}