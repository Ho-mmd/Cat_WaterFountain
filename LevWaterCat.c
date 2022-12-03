#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile unsigned int start=0, end=0, dist=0;
volatile unsigned int ABLE = 0;

int main(){

	cli();

	DDRB = 0xFF; // Output
	DDRF = 0xFF; // Trig
	DDRD = 0x00; // Ehco
	EICRA = (0<<ISC31) | (0<<ISC30) | (0<<ISC21) | (0<<ISC20) | (1<<ISC11) | (0<<ISC11) | (0<<ISC01) | (0<<ISC00); // 초기값 = 라이징엣지

	EIMSK = (0<<INT7) | (0<<INT6) | (0<<INT5) | (0<<INT4) | (0<<INT3) | (0<<INT2) | (1<<INT1) | (0<<INT0);

	TIMSK = 0x04;
	TCCR1A = 0X00;
	TCCR1B = 0X05;

	TCNT1H = 0xFF;
	TCNT1L = 0xFE;

	sei();

	do{


	}while(1);



}


ISR(INT1_vect){ //ehco
   	cli();

	if(EICRA == 0x0C){    // 라이징엣지에서 인터럽트가 걸리면
        start = TCNT1; // TCNT1값을 저장해두고
        EICRA = 0x08;     // 다음번 인터럽트는 폴링엣지로 설정
    }

	else{            // 폴링엣지에서 인터럽트가 걸리면
        end = TCNT1;   // TCNT1값을 저장해두고
        EICRA = 0x0C;     // 다음번 인터럽트는 라이징엣지로 설정
        dist= (unsigned int)((float)(end-start) / 14.5 ); // Sound_velocity -> 29us per 1cm -> [distance = duration / 29us / 2]

		if(dist <= 1) { // Dist Price 0->10cm, 1->20cm....
			ABLE = 1;
			PORTB = 0x10;
		} 
		else {
			PORTB = 0x00;
			ABLE = 0;
		}
    }

   sei();
}



ISR(TIMER1_OVF_vect){ // 일정시간마다 초음파센서 Trig 발생
   	cli();

   	TCNT1H = 0xE1;
   	TCNT1L = 0x7A;

   	PORTF = 0xFF;
	_delay_us(10);

	PORTF = 0x00;

   	sei();



}
