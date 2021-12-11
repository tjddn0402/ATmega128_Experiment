#include<avr/io.h>
#include<util/delay.h>
int main(void){
	unsigned char key;
	DDRA = 0b10000000;
	DDRE = 0b00000000;
	PORTE = 0b11111111;
	while(1) {
		key = PINE;
		switch (key) {
			case 0b11111110: // 도
			PORTA = 0b10000000;
			_delay_us(1908*4);
			PORTA = 0b00000000;
			_delay_us(1908*4);
			break;
			case 0b11111101: // 레
			PORTA = 0b10000000;
			_delay_us(1700*4);
			PORTA = 0b00000000;
			_delay_us(1700*4);
			break;
			case 0b11111011: // 미
			PORTA = 0b10000000;
			_delay_us(1515*4);
			PORTA = 0b00000000;
			_delay_us(1515*4);
			break;
			case 0b11110111: // 파
			PORTA = 0b10000000;
			_delay_us(1432*4);
			PORTA = 0b00000000;
			_delay_us(1432*4);
			break;
			case 0b11101111: // 솔
			PORTA = 0b10000000;
			_delay_us(1275*4);
			PORTA = 0b00000000;
			_delay_us(1275*4);
			break;
			case 0b11011111: // 라
			PORTA = 0b10000000;
			_delay_us(1136*4);
			PORTA = 0b00000000;
			_delay_us(1136*4);
			break;
			case 0b10111111: // 시
			PORTA = 0b10000000;
			_delay_us(1012*4);
			PORTA = 0b00000000;
			_delay_us(1012*4);
			break;
			case 0b01111111: // 도
			PORTA = 0b10000000;
			_delay_us(954*4);
			PORTA = 0b00000000;
			_delay_us(954*4);
			break;
			default:
			break;
		}
	}
}