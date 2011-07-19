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
	clear_bit(P1OUT,LED);
	
	//enable S2
	set_bit(P1REN,S2);
	set_bit(P1OUT,S2);
}

void error() {
	unsigned int i;
	while(1) {
		for(i=0;i<100;i++)
			__delay_cycles(32000);
		toggle_bit(P1OUT,LED);
	}
}

inline void cicad_send_byte(unsigned char byte) {
	char i;
	unsigned char b;
	
	//one by one send a bit from the byte
	for(i=7; i>=0; i--) {
		b=test_bit(byte,i);
		CICAD_WAIT_NEXT_BIT;
		cicad_send_bit(b);
	}
	
	//send sync bit, inverse of the last one
	CICAD_WAIT_NEXT_BIT;
	cicad_send_bit(~b);
}

int main(void)
{
	init_chip();
	init_io();
	cicad_physical_init();
	
	cicad_set_period(0x0c00);
	CICAD_SET_TIMER(cicad_1_period);
	cicad_init_timer(1);
	
	while(1) {
		cicad_send_byte(0b10101010);
	}
}
