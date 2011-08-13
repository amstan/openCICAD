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

///Buffer
volatile unsigned char buffer[100];
volatile unsigned char nb;
volatile unsigned char ib;
volatile char ibit;

///States
volatile enum State {
	clear,
	som,
	id,
	ctrl,
	data,
	crc,
	ack,
	eom,
} state;
unsigned char lastbit;
unsigned char write;
unsigned char halfperiod;

int main(void)
{
	//inits
	init_chip();
	init_io();
	cicad_physical_init();
	
	cicad_calculate_period(0x624); //0x624=100us
	CICAD_SET_TIMER(cicad_0_5_period);
	
	//prepare the buffer
	nb=ib=0;
	buffer[nb++]=0b00000000;
	buffer[nb++]=0b10100011;
	buffer[nb++]=0b11111111;
	buffer[nb++]=0b10100011;
	buffer[nb++]=0b11111111;
	
	//init the states
	ibit=7;
	write=1;
	halfperiod=0;
	
	cicad_init_timer(1);
	
	while(1) {
		__delay_random();
	}
}

unsigned char cicad_send_message(unsigned long id, unsigned char n, unsigned char *message) {
	
}

CICAD_TIMER_INTERRUPT_VECTOR cicad_timer_vector() {
	if(write) {
		if(!halfperiod) {
			//Point where the signal should change
			
			//Output the proper bit
			if(ibit>=0) {
				lastbit=test_bit(buffer[ib],ibit);
				cicad_send_bit(lastbit);
				ibit--;
			} else {
				//Output the byte limiter
				lastbit^=1; //byte delimiter is just the lastbit inverted
				cicad_send_bit(lastbit);
				ibit=7;
			}
			
			//Increment the byte index
			if(ibit==7) {
				ib++;
				ib%=nb;
			}
		} else {
			//Middle of the signal transmission
			
			//Check if a recessive bit is actually recessive or someone else is transmitting
			if(lastbit!=cicad_read_bit()) {
				//Loss of priority here
				
				///@TODO add priority loss message instead of the setting the led
				set_bit(P1OUT,LED);
			}
		}
		
		halfperiod^=1;
	}
}
