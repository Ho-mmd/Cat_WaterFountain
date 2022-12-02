#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

volatile unsigned char cnt = 0;
volatile unsigned int flag = 0;
volatile unsigned int ADC_Result;
volatile unsigned char ADC_Low, ADC_High;

//LED(v)

int main(void) {
	
	cli();

	DDRF = 0x00; //AD input
	DDRB = 0xFF;
	DDRD = 0xFF;	

	TCCR0 = (0<<FOC0) | (1<<WGM00) | (1<<COM01) | (1<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (1<<CS00 ); //PWM

	sei();

	TIMSK = 0x04;
	TCCR1A = 0x00; 
	TCCR1B = 0x05; //clk select

	TCNT1H = 0xFF; //초기에 overflow 발생시기 당김
	TCNT1L = 0xC0;

	ADMUX = 0x00;
	ADCSRA = 0x83;
	
	do {
		
	} while (1);

	return 0;
}

ISR(TIMER1_OVF_vect) { 
	cli();

	TCNT1H = 0xFE; //0.02초 (20ms, 50Hz, duty cycle = 5%)
	TCNT1L = 0xC6;

	ADCSRA |= 0x40; //ADC start, (1<<ADSC)

	while(ADCSRA&0x40); //Wait until ADCSRA & 0x40 != 0, (1<<ADSC)

	ADC_Low = ADCL;  //Low value First
	ADC_High = ADCH;
	ADC_Result = (ADC_Low + ADC_High * 256) / 30;

	flag++;

	if(flag < ADC_Result) {
		PORTD = 0x10;
	} else {
		PORTD = 0x00;
		flag = 0;
	}

	OCR0 = 190; //Water Pump Power Control

	sei();

}
