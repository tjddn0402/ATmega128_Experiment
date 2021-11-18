#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void USART0_Init(){
	UCSR0A = 0x00;
	UCSR0B = 0b10011000; // RXCIE0=1, TXEN0=1, RXEN0=1
	UCSR0C = 0b10000110; // 비동기 통신, 패리티 없음. 데이터 비트 : 8비트, 정지비트 : 1비트
	UBRR0H = 0;
	UBRR0L = 103; // fosc=14.7456MHz, BAUD=9600bps
}

void TimerCounter2_Init(){
	TCCR2=0b01101101;//분주비 1024, 주파수 16000000/256/1024=61Hz
	TIMSK=0b11000000;
	OCR2=14;
	TCNT2=0X00;
}

volatile unsigned char ch;
volatile unsigned char degree=45;
ISR(USART0_RX_vect){
	ch=UDR0;
	
	if(ch=='\r'){
		OCR2 = (2*degree)/23+15;
		degree=0;
	}
	else{
		degree=degree*10+ch-'0';
	}
}
ISR(TIMER2_COMP_vect){}
ISR(TIMER2_OVF_vect){}

int main(void)
{
	DDRB=0xff;
	
	TimerCounter2_Init();
	USART0_Init();
	SREG|=0x80;
	
	while (1)
	{
	}
}