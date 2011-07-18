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
	
	while(1) {
		cicad_send_bit(switch_pressed());
	}
}