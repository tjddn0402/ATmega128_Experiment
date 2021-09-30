#define F_CPU 1600000UL
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

#define LED PORTA

volatile unsigned char led_on;
volatile unsigned char period;

void wait();//_delay_ms 함수는 변수 파라미터를 못받으므로 wait으로 속도 조절

ISR(INT5_vect){	period++;}// 3번 스위치 : period up -> speed down
ISR(INT6_vect){//2번 스위치 : period down -> speed up
	if(period>1)
		period--;
}
ISR(INT7_vect){// 1번 스위치 누르면 on/off
	if (led_on) {
		led_on = 0;
	}
	else {
		led_on = 1;
	}
}

int main(void){
	unsigned char pos;
	period=1;
	
	DDRA = 0b11111111; // 출력 설정
	LED=0b00000000;
	DDRE = 0b00000000; // 입력 설정
	PORTE = 0b11100000; // 풀업저항 설정
	
	EICRB = 0b10101000; //인터럽트 트리거 방식 설정(falling edge)
	EIMSK = 0b11100000; //인터럽트 허용 설정
	SREG |= 0x80; //전체 인트럽트 허가
	led_on = 1;
	
	while(1) {
		if(led_on==1){
			for (pos=0xfe;pos!=0x7f;pos=((pos<<1)|0x01)){
				if(led_on==0){
					LED=0XFF;
					break;
				}
				wait();
				LED=pos;
			}
			for (;pos!=0xfe;pos=((pos>>1)|0x80)){
				if(led_on==0){
					LED=0XFF;
					break;
				}
				wait();
				LED=pos;
			}
		}
	}
}

void wait(){//period 값 통해 속도 조절
	unsigned char i;
	for(i=0;i<period;i++){
		_delay_ms(100);
	}
}