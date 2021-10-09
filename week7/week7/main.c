//GPIO, USART, LCD, INTERRUPT, TIMER 모두 사용
//issue: 터미널에서 아스키 스트링(최대 3글자)으로 입력한 것을 그대로 출력하고, 숫자로 변환한 것은 스위치 입력과 비교해야 한다.
//string을 stdio함수중 적절한 것을 통해 입력받아야 한다.
//엔터와 동시에 타이머 카운터 켜서 시간 측정한다.
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define LED PORTF

//LCD 제어 함수 및 전처리, 출력할 STRING
#define CTRL PORTA
#define INST PORTB
#define DATA PORTB
#define E 0x04
#define RW 0x02
#define RS 0x01
void cmd(int command);
void data(char str);
void display(volatile char *str, unsigned char line);
volatile char screen[6][17] = {{"8bit Binary Game"}, {"   : 5 sec left"},
							{"Correct!"}, {"Game Over     "},
							{"                "}, {"waiting.."}};
volatile unsigned char timeLimit = 5;

//USART 인터럽트
volatile unsigned char gameState = 0x00;//0: 게임중 여부
										//1: 사용자 pc 입력이 끝났는가
volatile unsigned char ch;				 //PC유저가 입력한 문자를 받는 변수
volatile unsigned char inputNum = 0;
volatile unsigned char numPos = 0;
ISR(USART0_RX_vect){
	if (gameState & 0x01){
		if ((ch = UDR0) != 'q'){
			screen[1][numPos] = ch;
			numPos++;
			inputNum *= 10;
			inputNum += (ch - '0');
			display(screen[1], 2);
		}
		else{
			gameState |= 0x02;
			display(screen[4], 2);
			numPos = 0;
			TCNT2 = 0x00; //엔터키 받고부터 정확한 시간 재기
		}
	}
}

//외부 인터럽트
volatile unsigned char binNum = 0x00;
ISR(INT0_vect){	if (gameState & 0x01){binNum |= 0x01;LED |= 0x01;}
				else{	gameState |= 0x01;	LED = 0x00;	display(screen[5], 2);}} //게임 시작
ISR(INT1_vect){	if (gameState & 0x01){binNum |= 0x02;LED |= 0x02;}
				else{	if (timeLimit < 9)	LED = ++timeLimit;}} //게임 시간 조절
ISR(INT2_vect){	if (gameState & 0x01){binNum |= 0x04;LED |= 0x04;}
				else{	if (timeLimit > 1)	LED = --timeLimit;}}
ISR(INT3_vect){	if (gameState & 0x01){binNum |= 0x08;LED |= 0x08;}}
ISR(INT4_vect){	if (gameState & 0x01){binNum |= 0x10;LED |= 0x10;}}
ISR(INT5_vect){	if (gameState & 0x01){binNum |= 0x20;LED |= 0x20;}}
ISR(INT6_vect){	if (gameState & 0x01){binNum |= 0x40;LED |= 0x40;}}
ISR(INT7_vect){	if (gameState & 0x01){binNum |= 0x80;LED |= 0x80;}}

//타이머/카운터 인터럽트//if 문 내에서 항상 binNum과 입력 값을 비교한다.
volatile unsigned char count;
ISR(TIMER2_OVF_vect){
	if (gameState & 0x02){ //터미널 입력이 끝나서 플레이어의 입력을 기다린다면
		if (binNum != inputNum){//아직 플레이어의 입력이 출제가의 입력과 다르다면
			count++;
			if (count == 60){
				if ((--screen[1][5]) >= '0'){display(screen[1], 2);}
				else{ //time over
					display(screen[3], 2);
					_delay_ms(500);
					LED = binNum = inputNum = gameState = 0x00; //게임 끝
					display(screen[4], 2);
				}
				count = 0;
			}
		}
		else{ //correct
			display(screen[2], 2);
			LED = binNum = inputNum = gameState = 0x00; //게임 끝
			_delay_ms(50);
			display(screen[4], 2);
		}
	}
}

int main(void){
	DDRA = 0xff;
	DDRB = 0XFF;
	DDRD = 0X00;
	DDRE = 0X00;
	DDRF = 0XFF;
	PORTD = 0X0F;
	PORTE = 0XF0;
	LED = 0X00;
	//USART 관련 레지스터
	UCSR0A = 0x00;
	UCSR0B = 0b10011000;
	UCSR0C = 0b10000110;
	UBRR0H = 0;
	UBRR0L = 103;
	//외부인터럽트 관련 레지스터
	EICRA = 0b10101010;
	EICRB = 0b10101010; //인터럽트 트리거 방식 설정(falling edge)
	EIMSK = 0b11111111; //인터럽트 허용 설정
	//타이머/카운터2 관련 레지스터
	TCCR2 = 0b00000101; //프리스케일러 분주비 1024
	TIMSK = 0b01000000; //오버플로우 인터럽트 허용
	TCNT2 = 0x00;		//타이머카운터0 초기화

	SREG = 0x80; //전체 인터럽트 허용

	//LCD 초기화
	cmd(0b00111000); // Function Set. Set 8bit 1Line 5x7 dots
	cmd(0b00000001); // Display clear.
	cmd(0b10000000); // Set DDRAM address or cursor position on display
	cmd(0b00001100); // Display on. Cursor Off. Blink Off
	cmd(0b00000110); // Entry Mode. Cursor Increment
	display(screen[0], 1);

	while (1){}
}

void cmd(int command){
	INST = command;
	CTRL = E;
	_delay_ms(1);
	CTRL &= ~(E);
}

void data(char str){
	DATA = str;
	CTRL = (E | RS);
	_delay_ms(1);
	CTRL &= ~(E);
}

void display(volatile char *str, unsigned char line){ // print selected line on LCD
	if (line == 1){cmd(0b10000000);} //select 1st line
	else{cmd(0b11000000);} //select 2nd line
	int i = 0;
	while (str[i] != '\0'){data(str[i++]);}
}