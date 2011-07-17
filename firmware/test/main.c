#include <msp430.h>
#include "bitop.h"

#define LED1 0
#define LED2 6
#define BUS  LED2
#define LED  LED1
#define S2   3

inline unsigned char read_bus() {
	return test_bit(P1IN,BUS);
}

inline void send_bus(unsigned char val) {
	change_bit(P1OUT,LED,val);
	change_bit(P1DIR,BUS,val);
}

inline unsigned char switch_pressed() {
	return !test_bit(P1IN,S2);
}
 
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	DCOCTL  = CALDCO_16MHZ; // Load the clock calibration
	BCSCTL1 = CALBC1_16MHZ;
	
	//Configure ports
	P1REN=0; P1OUT=0; P1DIR=0;
	
	set_bit(P1DIR,LED); //enable led
	
	//enable S2
	set_bit(P1REN,S2);
	set_bit(P1OUT,S2);
	
	//output all the time on the bus, only toggle the DIR
	set_bit(P1OUT,BUS);
	
	while(1) {
		send_bus(switch_pressed());
	}
}