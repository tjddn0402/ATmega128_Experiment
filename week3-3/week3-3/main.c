#define F_CPU 1600000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

volatile char ch;

ISR(USART0_RX_vect){
	ch=UDR0;
	UDR1=ch;
}

ISR(USART1_RX_vect){
	ch=UDR1;
	UDR0=ch;
}

int main(void)
{
	UCSR0A|=0b00000000;
	UCSR0B|=0b10011000;
	UCSR0C|=0b10000110;
	UCSR1A|=0b00000000;
	UCSR1B|=0b10011000;
	UCSR1C|=0b10000110;
	
	UBRR0H=0;
	UBRR0L=103;
	UBRR1H=0;
	UBRR1L=103;
	SREG=0x80;
	
	/* Replace with your application code */
	while (1)
	{
	}
}