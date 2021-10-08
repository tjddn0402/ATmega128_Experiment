#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CTRL PORTA
#define DATA PORTB //data 또는 insturction
#define INST PORTB
#define SWITCH PORTD
#define RS	0x01
#define RW	0x02
#define E	0x04

void cmd(int command);
void data(char str);
void display(volatile char *str,unsigned char line);

volatile unsigned int count = 0;
volatile char str[2][16]={{"Stop Watch"},{"00:00:00"}};
void tick();// manipulate str[1] to increase time on LCD
volatile unsigned char goStop=0;//1:count time // 0:stop
	
ISR(TIMER2_OVF_vect)
{
	if(goStop){
		count++;
		if (count == 60){
			tick();
			display(str[1],2);
			count = 0;
		}
	}
}

ISR(INT0_vect){goStop=1;}//start
ISR(INT1_vect){goStop=0;}//stop
ISR(INT4_vect){//initialize
	str[1][0]=str[1][1]=str[1][3]=str[1][4]=str[1][6]=str[1][7]='0';
	display(str[1],2);
	goStop=0;
}

int main(void)
{
    DDRA=0xff;
	DDRB=0xff;
	DDRD=0x00;
	
	SWITCH=0b00000111;//풀업저항 사용
	
	EICRA=0b00101010;// FALLING edge trigger
	EICRB=0b00000010;
	EIMSK=0b00010011;// 외부인터럽트 허용
	
	TCCR2 = 0b00000101; //프리스케일러 분주비 1024
	TIMSK = 0b01000000; //오버플로우 인터럽트 허용
	TCNT2 = 0x00;		//타이머카운터0 초기화
	
	SREG = 0x80;		//전체 인터럽트 허가
	
	cmd(0b00111000); // Function Set. Set 8bit 1Line 5x7 dots
	cmd(0b00000001); // Display clear.
	cmd(0b10000000); // Set DDRAM address or cursor position on display
	cmd(0b00001100); // Display on. Cursor Off. Blink Off
	cmd(0b00000110); // Entry Mode. Cursor Increment
	
	display(str[0],1);
	display(str[1],2);
	
	while (1) 
    {
    }
}

void cmd(int command)
{
	INST = command;
	CTRL = E;
	_delay_ms(1);
	CTRL &= ~(E);
}
void data(char str)
{
	DATA = str;
	CTRL = (E|RS);
	_delay_ms(1);
	CTRL &= ~(E);
}

void display(volatile char *str,unsigned char line)
{	// print selected line on LCD
	if(line==1){cmd(0b10000000);}	//select 1st line
	else{cmd(0b11000000);}			//select 2nd line
	int i = 0;
	while (str[i] != '\0'){data(str[i++]);}
}

void tick(){
	if (str[1][7]=='9'){str[1][7]='0';
		if(str[1][6]=='5'){str[1][6]='0';
			if(str[1][4]=='9'){str[1][4]='0';
				if(str[1][3]=='5'){str[1][3]='0';
					if(str[1][1]=='9'){	str[1][1]='0';
						if(str[1][0]=='9'){str[1][0]='0';}
						else{str[1][0]++;}
					}else{str[1][1]++;}
				}else{str[1][3]++;}
			}else{str[1][4]++;}
		}else{str[1][6]++;}
	}else{str[1][7]++;}
}