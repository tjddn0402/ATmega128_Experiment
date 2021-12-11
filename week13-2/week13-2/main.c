#define F_CPU 16000000
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

volatile unsigned char on = 0;
volatile unsigned int count = 0;
ISR(TIMER2_COMP_vect){
	if(on){
		PORTA^=0x80;
	}
}

int main(void){
	unsigned char key;
	DDRA = 0b10000000;
	DDRE = 0b00000000;
	PORTE = 0b11111111;
	
	TCCR2 = 0b00001100; // CTC모드, 프리스케일러 분주비 64
	TIMSK = 0b10000000; // 타이머2의 비교일치 인터럽트 설정
	OCR2 = 124; // 비교일치 값 지정
	TCNT2 = 0; // 초기값
	SREG = 0x80;
	
	while(1) {
		key = PINE;
		switch (key) {
			case 0b11111110: // 도
			OCR2=118;// 16MHz/(2*N*f)-1
			on=1;
			break;
			case 0b11111101: // 레
			OCR2=106;
			on=1;
			break;
			case 0b11111011: // 미
			OCR2=94;
			on=1;
			break;
			case 0b11110111: // 파
			OCR2=89;
			on=1;
			break;
			case 0b11101111: // 솔
			OCR2=79;
			on=1;
			break;
			case 0b11011111: // 라
			OCR2=70;
			on=1;
			break;
			case 0b10111111: // 시
			OCR2=62;
			on=1;
			break;
			case 0b01111111: // 도
			OCR2=59;
			on=1;
			break;
			default:
			on=0;
			break;
		}
	}
}