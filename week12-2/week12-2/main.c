#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

typedef unsigned char Byte;

#define X_data		0
#define Y_data		2
#define Z_data		4
#define g_Select1	5	//00
#define g_Select2	6
#define SLP			7	//1

void ADC_SingleConversion_Init(unsigned char port) {ADMUX = port;} //ADMUX값을 설정

int ADC_SingleConversion_read() {
	int output;//10bit data이므로 int형으로 받는다
	ADCSRA |= (1<<ADSC) | (1<<ADEN) ; //Start single conversion
	while(ADCSRA & (1<<ADSC)); //wait Conversion to complete
	output = ADCL + (ADCH << 8);
	ADCSRA &= ~(1<<ADEN); //disable ADC
	return output;
}

void revise_str(char *str){
	int x,y,z;
	ADC_SingleConversion_Init(X_data);
	x=ADC_SingleConversion_read();
	ADC_SingleConversion_Init(Y_data);
	y=ADC_SingleConversion_read();
	ADC_SingleConversion_Init(Z_data);
	z=ADC_SingleConversion_read();
	
	sprintf(str,"X-axis :%4d  Y-axis :%4d  Z-axis :%4d\r",x,y,z);// \r, \n 중 어떤게 맞는지 테스트
}

void display_USART(char *str){
	Byte i=0;
	while(str[i]!='\0'){
		UDR0=str[i];
		i++;
		_delay_ms(1);
	}
}

int main(void)
{
	char str[45];
	DDRF=0b11100000;//가속도센서 제어비트만 1(출력), 나머지는 ADC 0(입력)
	PORTF=(1<<SLP)|(~(1<<g_Select1))|(~(1<<g_Select2));
	
	UCSR0A = 0x00;
	UCSR0B = 0b10011000; // RXCIE0=1, TXEN0=1, RXEN0=1
	UCSR0C = 0b10000110; // 비동기 통신, 패리티 없음, 데이터 비트 : 8비트, 정지비트 : 1비트
	UBRR0H = 0;
	UBRR0L = 103; // fosc=14.7456MHz, BAUD=9600bps
	
	SREG=0X80;
	
    while (1){//1초마다 센서값 갱신하고 USART통해 출력
		revise_str(str);
		display_USART(str);
		_delay_ms(1000);
    }
}

