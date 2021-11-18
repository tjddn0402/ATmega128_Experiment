#define F_CPU 16000000
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

void cmd(int);
void data(char);
void display1(char*);
void display2(char*);
char str[2][16] = {{"ADC VALUE :"},{" "}};
	
void ADC_SingleConversion_Init(unsigned char port) {
	ADMUX = port; //ADMUX값을 설정
}
int ADC_SingleConversion_read() {
	int output;
	ADCSRA |= (1<<ADSC) | (1<<ADEN) ; //Start single conversion
	while(ADCSRA & (1<<ADSC)); //wait Conversion to complete
	output = ADCL + (ADCH << 8);
	ADCSRA &= ~(1<<ADEN); //disable ADC
	return output;
}void cmd(int command){
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


int main()
{
	int ADC_Output;
	float volt;
	DDRA = 0xff; // set port A as output
	DDRB = 0xff; // set port B as output
	DDRF = 0xfe; // set port F as intput
	EICRA = 0xff;
	EIMSK = 0x0f;
	SREG = 0x80;
	cmd(0x38);
	cmd(0x01);
	cmd(0x80);
	cmd(0x0c);
	cmd(0x06);
	//위 내용은 LCD강의 참고
	while (1){
		display1(str[0]); // 1행 출력
		display2(str[1]); // 2행 출력
		ADC_SingleConversion_Init(0x00); // ADC0 설정
		ADC_Output = ADC_SingleConversion_read(); // 변환된 digtal 값 읽기
		volt = 5.*(ADC_Output/1023.); // 가변저항으로 얻은 전압값 계산
		sprintf(str[1],"%d.%d%d%dV",((int)volt),((int)(volt*10))%10,
		((int)(volt*100))%10,((int)(volt*1000))%10); // 전압 값 출력
		_delay_ms(10);
	}
	return 0;
}