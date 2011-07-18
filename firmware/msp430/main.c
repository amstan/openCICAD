#include <msp430.h>

#include "cicad_physical.h"
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
	//enable led
	set_bit(P1DIR,LED);
	
	//enable S2
	set_bit(P1REN,S2);
	set_bit(P1OUT,S2);
}

int main(void)
{
	init_chip();
	init_io();
	cicad_physical_init();
	
	cicad_set_period(0x8000);
	CICAD_SET_TIMER(cicad_1_period);
	cicad_init_timer(1);
	
	unsigned int i;
	
	while(1) {
		while(!CICAD_CHECK_TIME) {
			if(switch_pressed()) {
				CICAD_TIMER_RESET;
			}
		}
		CICAD_RESET_CHECK_TIME;
		
		i++;
		i%=488; //488hz=16mhz/0x8000
		if(i==0) {
			toggle_bit(P1OUT,LED);
		}
	}
}