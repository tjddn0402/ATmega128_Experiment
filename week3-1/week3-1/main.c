#define F_CPU 1600000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
volatile unsigned char ch;
ISR(USART0_RX_vect){
	ch = UDR0 ;
	PORTA = ch-'0';
}

int main(void)
{
	DDRA = 0xff ;
	UCSR0A = 0x00;
	UCSR0B = 0b10011000; // RXCIE0=1, TXEN0=1, RXEN0=1
	UCSR0C = 0b10000110; // 비동기 통신, 패리티 없음,
	// 데이터 비트 : 8비트, 정지비트 : 1비트
	UBRR0H = 0;
	UBRR0L = 103; // fosc=14.7456MHz, BAUD=9600bps
	SREG = 0x80;
	while(1);
}