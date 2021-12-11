#define F_CPU 16000000
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

volatile unsigned char octave=1;
volatile unsigned char on = 0;
volatile unsigned int count = 0;
///////////////////////////////////////
void cmd(int command){
	PORTB = command;
	PORTA= 0x04;
	_delay_ms(1);
	PORTA= 0x00;
}
void data(char str){
	PORTB= str;
	PORTA= 0x05;
	_delay_ms(1);
	PORTA= 0x01;
}
void display1(char *str){
	cmd(0x80);
	int i =0;
	while(str[i]!='\0'){
		data(str[i++]);
	}
}
void display2(char *str){
	cmd(0xc0);
	int i =0;
	while(str[i]!='\0'){
		data(str[i++]);
	}
}
void display_Init(){
	cmd(0x38);
	cmd(0x01);
	cmd(0x80);
	cmd(0x0c);
	cmd(0x06);
}
char str[5][16]={"2 octave",}
///////////////////////////////////////
ISR(TIMER2_COMP_vect){
	if(on){PORTA^=0x80;}
}
ISR(USART1_)
///////////////////////////////////////
void ExternInt_Init(){
	DDRE=0x00;
	PORTE=0b00110000;
	EIMSK=0b00110000;
	EICRB=0b00001010;
}
ISR(INT4_vect){
	if(octave!=2){
		octave++;
		str[0][0]++;
	}
	display1(str[0]);
}
ISR(INT5_vect){
	if(octave!=0){
		octave--;
		str[0][0]--;
	}
	display1(str[0]);
}
///////////////////////////////////////
void ADC_Init();
///////////////////////////////////////
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
		if (octave==0){
			switch(key){
				case 0b11111110: // 도
				OCR2=59;// 16MHz/(2*N*f)-1
				on=1;
				break;
				case 0b11111101: // 레
				OCR2=53;
				on=1;
				break;
				case 0b11111011: // 미
				OCR2=47;
				on=1;
				break;
				case 0b11110111: // 파
				OCR2=44;
				on=1;
				break;
				case 0b11101111: // 솔
				OCR2=39;
				on=1;
				break;
				case 0b11011111: // 라
				OCR2=35;
				on=1;
				break;
				case 0b10111111: // 시
				OCR2=31;
				on=1;
				break;
				case 0b01111111: // 도
				OCR2=29;
				on=1;
				break;
				default:
				on=0;
				break;
			}
		}
		else if(octave==2){
			switch (key) {
				case 0b11111110: // 도
				OCR2=236;// 16MHz/(2*N*f)-1
				on=1;
				break;
				case 0b11111101: // 레
				OCR2=212;
				on=1;
				break;
				case 0b11111011: // 미
				OCR2=188;
				on=1;
				break;
				case 0b11110111: // 파
				OCR2=178;
				on=1;
				break;
				case 0b11101111: // 솔
				OCR2=158;
				on=1;
				break;
				case 0b11011111: // 라
				OCR2=140;
				on=1;
				break;
				case 0b10111111: // 시
				OCR2=124;
				on=1;
				break;
				case 0b01111111: // 도
				OCR2=118;
				on=1;
				break;
				default:
				on=0;
				break;
			}
		}
		else{
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
}
