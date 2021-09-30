/*
 * week5-1.c
 *
 * Created: 2021-09-24 오후 5:07:40
 * Author : KSW
 */ 
#define F_CPU 1600000UL
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

#define LED PORTB

unsigned char led_on;


ISR(INT3_vect){
	if (led_on) {
		LED|=0b10000000; // LED OFF
		led_on = 0;
	}
	else {
		LED&=~(1<<7); // LED ON
		led_on = 1;
	}
}

int main(void){
	DDRB = 0b10000111; // 출력 설정
	DDRD = 0b00000000; // 입력 설정
	LED=0b00000111;
	PORTD = 0b00001000; // 풀업저항 설정
	EICRA = 0b10000000; //인터럽트 트리거 방식 설정(falling edge)
	EIMSK = 0b00001000; //인터럽트 허용 설정
	SREG |= 0x80; //전체 인트럽트 허가
	led_on = 1;
	while(1) {
		LED|=0b00000111; // LED ON
		_delay_ms(500);
		LED&=0b10000000; // 모든 LED OFF
		_delay_ms(500);
	}
}

