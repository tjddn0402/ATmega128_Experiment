#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char speed=0b10000000;
volatile unsigned char power=1;
volatile unsigned char direction;

void timer_counter0_init(){
	TCCR0=0b01101111;//FAST PWM, NON-INVERTING(), 1024 PRESCALER
	TIMSK=0B00000011;
	OCR0=speed;
	TCNT0=0x00;
}
void extern_interrupt_B_init(){
	EICRB=0b10101010;//FALLING EDGE TRIGGERED
	EIMSK=0b11110000;//INT4,5,6,7
}

ISR(INT4_vect){//회전/멈춤
	if(power==1){
		DDRB=0;
		power=0;
	}
	else{
		DDRB=0xff;
		power=1;
	}
}
ISR(INT5_vect){//회전방향 변화
	direction=PORTF;
	PORTF=~direction;
}
ISR(INT6_vect){//회전 속도 증가
	speed=OCR0;
	OCR0=speed+16;
}
ISR(INT7_vect){//회전 속도 감소
	speed=OCR0;
	OCR0=speed-16;
}

ISR(TIMER0_COMP_vect){}
ISR(TIMER0_OVF_vect){}

int main(void)
{
	DDRF=0XFF;
	DDRB=0XFF;
	DDRE=0X00;//스위치 입력
	
	PORTE=0XFF;//PULL-UP저항
	PORTF=0b00000001;
	
	timer_counter0_init();
	extern_interrupt_B_init();
	SREG|=0X80;
	
    while (1) 
    {
    }
}

