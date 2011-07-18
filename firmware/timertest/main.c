#include <msp430.h>

#include "bitop.h"
#include "debug.h"

#include "config.h"

inline unsigned char switch_pressed() {
	return !test_bit(P1IN,S2);
}

void init_chip() {
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	DCOCTL  = CALDCO_16MHZ; // Load the clock calibration
	BCSCTL1 = CALBC1_16MHZ;
}

void init_io() {
	set_bit(P1DIR,LED); //enable led
	
	//enable S2
	set_bit(P1REN,S2);
	set_bit(P1OUT,S2);
}

void init_timer() {
	TACTL = TASSEL_2 | ID_0 | MC_1;
	TACCR0 = 0x8000; //Count up to
}

int main(void)
{
	init_chip();
	init_io();
	init_timer();
	
	unsigned long i = 0;
	
	while(1) {
		while(!(TACCTL0 & CCIFG)) {
			if(switch_pressed()) {
				//restart the timer
				TAR=0;
			}
		}
		TACCTL0&=~CCIFG;
		
		i++;
		i%=488; //488hz=16mhz/0x8000
		if(i==0) {
			toggle_bit(P1OUT,LED);
		}
	}
}