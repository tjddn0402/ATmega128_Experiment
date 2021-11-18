#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

// CTC Mode
volatile unsigned char led = 0x0f; // 초기 led
volatile unsigned int count = 0;
ISR(TIMER2_COMP_vect){
	count++; // 인터럽트 횟수 카운트
	if(count == 125){ // 125번째 인터럽트시 (1초)
		PORTF = (~PORTF);// 반전 출력
		count = 0; // 카운트 초기화
	}
}
int main(){
	DDRF = 0xff; // LED 포트를 출력으로 설정
	TCCR2 = 0b00001101; // CTC모드, 프리스케일러 분주비 1024
	TIMSK = 0b10000000; // 타이머2의 비교일치 인터럽트 설정
	OCR2 = 124; // 비교일치 값 지정
	TCNT2 = 0; // 초기값
	SREG = 0x80; // 전역 인터럽트 허용
	PORTF = led;
	while(1){
	}
	return 0;
}