#include <msp430.h>
#include "bitop.h"

#define LED1 0
#define LED2 6
#define S2   3
 
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	DCOCTL  = CALDCO_16MHZ; // Load the clock calibration
	BCSCTL1 = CALBC1_16MHZ;
	
	//Configure ports
	P1DIR=(1<<LED1)|(1<<LED2);
	P1REN=(1<<S2);
	P1OUT=(1<<LED2)|(1<<S2);
	
	while(1) {
		change_bit(P1OUT,LED1,test_bit(P1IN,S2));
		change_bit(P1OUT,LED2,test_bit(P1IN,S2));
	}
}