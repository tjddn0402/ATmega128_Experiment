#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
// Normal Mode
unsigned char led = 0x00 ;
unsigned int count = 0;

ISR(TIMER2_OVF_vect){
	count ++;
	if (count == 60) {
		if(led==0b10000000) {
			led=0b00000001;
		}
		else {
			led = led << 1;
		}
		count = 0;
	}
}

int main(){
	DDRA = 0xff ;
	TCCR2 = 0b00000101 ; //프리스케일러 분주비 1024
	TIMSK = 0b01000000 ; //오버플로우 인터럽트 허용
	TCNT2 = 0 ; //타이머카운터0 초기화
	SREG = 0x80 ; //전체 인터럽트 허가
	led=0b00000001;
	while(1){
		PORTA=led;
	}
}