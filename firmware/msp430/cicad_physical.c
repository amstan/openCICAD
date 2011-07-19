#include "bitop.h"
#include "debug.h"

#include "cicad_physical.h"

void cicad_physical_init() {
	clear_bit(P1DIR, CICAD_PIN); //Input
	clear_bit(P1REN, CICAD_PIN); //No resistor
	
	//Set the dominant bit to be outputted
	change_bit(P1OUT, CICAD_PIN, CICAD_POLARITY);
}

inline unsigned char cicad_read_bit() {
	#if CICAD_POLARITY == 1
		return test_bit(P1IN, CICAD_PIN);
	#else
		return ~test_bit(P1IN, CICAD_PIN);
	#endif
}

inline void cicad_send_bit(unsigned char val) {
	#ifdef DEBUG
		change_bit(P1OUT, LED, val); //Show if outputting on the led
	#endif
	
	change_bit(P1DIR, CICAD_PIN, val); //Enable outputting on the bus, therefore dominating it
}

void cicad_set_period(unsigned int period) {
	cicad_1_period=period;
	cicad_1_5_period=period+period/2;
}

void cicad_init_timer(unsigned char on) {
	TACTL |= TASSEL_2;      //Internal osc
	TACTL |= ID_0;          //1x prescaler
	change_bit(TACTL,4,on)  //Change the mode, on=1 => count up to taccr0, on=0 => stop
	
	CICAD_TIMER_RESET;      //Start the counter from 0
}