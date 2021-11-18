#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
int main(){
	DDRE = 0xff ;
	
	while(1){
		PORTE = 0b10110000;//enable=1//mode1,0=1//dir=0
		_delay_ms(1);
		PORTE = 0b10110101;
		_delay_ms(1);
	}
	return 0 ;
}