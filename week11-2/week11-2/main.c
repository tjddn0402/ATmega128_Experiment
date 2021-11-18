#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char dir=1;
volatile unsigned char period=2;
unsigned char mode = 0b1011;

ISR(INT0_vect){//속도 증가 == 주기 감소
	if(period!=1){period--;}
}
ISR(INT1_vect){//속도 감소
	period++;
}
ISR(INT2_vect){//회전방향 변화
	if(dir==1){dir=0;}
	else{dir=1;}
}

void extern_interrupt(){
	EICRA=0b00101010;//falling edge
	EIMSK=0b00000111;
}

void delay(){
	unsigned char i;
	for(i=0;i<period;i++){
		_delay_ms(1);
	}
}

int main(void)
{
	DDRD=0x00;//입력(외부인터럽트)
	DDRE=0xff;//출력
	PORTD=0xff;
	PORTE=0x33;
	
	extern_interrupt();
	
    while (1) 
    {
		if(dir==1){
			PORTE = 0b10110000;
			delay();
			PORTE = 0b10110101;
			delay();
		}
		else{
			PORTE = 0b10111010;
			delay();
			PORTE = 0b10111111;
			delay();
		}
    }
}