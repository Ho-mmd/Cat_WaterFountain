#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

volatile unsigned int flag = 1;

int main(void) {

	cli();

	DDRB = 0xFF;

	sei();	

	ETIMSK = 0x04;
	TCCR3A = 0x00; 
	TCCR3B = 0x05; //clk select

	TCNT3H = 0xFF; //초기에 overflow 발생시기 당김
	TCNT3L = 0xC0;
	
	do {
		
	} while (1);

	return 0;
}

ISR(TIMER3_OVF_vect) { 
	cli();

	TCNT3H = 0xFE; //0.02초 (20ms, 50Hz, duty cycle = 5%)
	TCNT3L = 0xC6;

	if(flag) {
		flag = 0;
		PORTB = 0xFF;
		_delay_ms(1);
			
	} else {
		flag = 1;	
		PORTB = 0x00;
		_delay_ms(19);
	}

	sei();

}
