#include <msp430.h>

#include "cicad.h"
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

void __delay_random() {
	unsigned int i;
	for(i=0;i<20;i++)
		__delay_cycles(31000);
}

void error() {
	while(1) {
		__delay_random();
		toggle_bit(P1OUT,LED);
	}
}

///Clears the interrupt flag for the pin interrupt.
#define CICAD_PIN_INTERRUPT_RESET clear_bit(P1IFG, CICAD_PIN);

void cicad_init_edge_interrupt() {
	set_bit(P1IE, CICAD_PIN); //Interrupt enable
	
	//Edge select
	change_bit(P1IES, CICAD_PIN, test_bit(P1IN, CICAD_PIN));
	
	//Enable global interrupt
	_BIS_SR(GIE); //Global
}

int main(void)
{
	//inits
	init_chip();
	init_io();
	cicad_init(0x624); //0x624=100us according to my scope
	cicad_init_edge_interrupt();
	
	cicad_send_bit(0);
	
	//unsigned char a[] = {0b11000011};
	
	while(1) {
		//cicad_send_message(10, (sizeof a), a);
	}
}

CICAD_PIN_INTERRUPT_VECTOR cicad_pin_interrupt() {
	CICAD_PIN_INTERRUPT_RESET;	
	//Switch to the other edge
	toggle_bit(P1IES, CICAD_PIN);
	
	set_bit(P1OUT,LED);
	__delay_cycles(100);
	clear_bit(P1OUT,LED);
}

