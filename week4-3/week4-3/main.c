#define F_CPU 1600000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PORTA	display
#define PORTB	message

ISR(){
	
}
ISR(){
	
}
ISR(){
	
}
ISR{(){
	
}

void cmd(int command)
{
	display = 0b00000000;//Enable=0
	_delay_ms(1);
	display = 0b00000100;//Enable=1
	_delay_ms(1);
	message = command;
	_delay_ms(1);
	display = 0b00000000;
	_delay_ms(1);
}

void data(char str)
{
	display = 0b00000101;
	_delay_ms(1);
	message = str;
	_delay_ms(1);
	display = 0b00000001;
	_delay_ms(1);
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

volatile char alphabets[1][27] = {{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"}}; // Initial LCD display

int main()
{
	unsigned int cursor=0;
	DDRA = 0b11111111;
	DDRB = 0b11111111;

	_delay_ms(50);
	cmd(0b00111000); // Function Set. Set 8bit 2Line 5x7 dots
	cmd(0b00001100); // Display on. Cursor Off. Blink Off
	cmd(0b00000001); // Display clear.
	cmd(0b10000000); // Set DDRAM address or cursor position on display
	cmd(0b00000110); // Entry Mode. Cursor Increment

	display1(alphabets[0]);
	cmd(0b00001111);
	cmd(0b00000101);// Cursor shift to right

	
	while (1)
	{
		if(cursor>=10){
			cursor=0;
			_delay_ms(10000);
			cmd(0b00000010);// Cursor Home
		}
		else{
			_delay_ms(10000);
			cmd(0b00011000);// Cursor shift to right
			cursor++;
		}
	}
}