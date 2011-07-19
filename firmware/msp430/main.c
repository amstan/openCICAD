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
	
	cicad_set_period(0x0c00);
	CICAD_SET_TIMER(cicad_1_period);
	cicad_init_timer(1);
	
	unsigned int i;
	unsigned char d =0;
	
	unsigned char a[] = {0,1,0,1,0,0,1,1,0,0,0,0,1,1,1,1,0,1};
	unsigned char na = 18;
	
	while(1) {
		cicad_send_bit(0);
		__delay_cycles(32000);
		
		CICAD_TIMER_RESET;
		for(i=0;i<na;i++) {
			cicad_send_bit(a[i]);
			
			CICAD_WAIT_NEXT_BIT;
		}
	}
}
