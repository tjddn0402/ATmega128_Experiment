/*
 * week5-1.c
 *
 * Created: 2021-09-24 오후 5:07:40
 * Author : KSW
 */ 
#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

#define LED PORTA


unsigned char led_on;
unsigned char period;

void wait();

ISR(INT5_vect){	period++;}
ISR(INT6_vect){	period--;}
ISR(INT7_vect){
	if (led_on) {
		led_on = 0;
	}
	else {
		led_on = 1;
	}
}

int main(void){
	unsigned char pos;
	period=5;
	
	DDRA = 0b11111111; // 출력 설정
	
	DDRE = 0b00000000; // 입력 설정
	PORTE = 0b11100000; // 풀업저항 설정
	
	EICRB = 0b10101000; //인터럽트 트리거 방식 설정(falling edge)
	EIMSK = 0b00001000; //인터럽트 허용 설정
	SREG |= 0x80; //전체 인트럽트 허가
	led_on = 1;
	
	while(1) {
		if(led_on==1)		{
			for (pos=0x01;pos<=0x80;pos<<=pos){
				if(led_on==0){break;}
				wait();
				LED=pos;
			}
			for (pos=0x80;pos>=0x01;pos>>=pos){
				if(led_on==0){break;}
				wait();
				LED=pos;
			}
		}
	}
}

void wait(){
	unsigned char i;
	for(i=0;i<period;i++){
		_delay_ms(100);
	}
}

