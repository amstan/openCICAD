#include "bitop.h"
#include "debug.h"

#include "cicad_physical.h"

///@file cicad_physical.c
///Bus configured to have +5 Dominant and 0 Recessive bit.
///@note Bus pulled down by LED and resistor on each launchpad.
void cicad_physical_init() {
	clear_bit(P1DIR, CICAD_PIN); //Input
	clear_bit(P1REN, CICAD_PIN); //No resistor
	set_bit(P1OUT, CICAD_PIN); //Always output 1
}

inline unsigned char cicad_read_bit() {
	return test_bit(P1IN, CICAD_PIN);
}

inline void cicad_send_bit(unsigned char val) {
#ifdef DEBUG
	change_bit(P1OUT, LED, val); //Show if outputting on the led
#endif
	change_bit(P1DIR, CICAD_PIN, val); //Enable outputting on the bus, therefore dominating it
}

void cicad_calculate_period(unsigned int period) {
	cicad_1_period=period;
	cicad_1_5_period=period+period/2;
}

void cicad_init_timer(unsigned char on) {
	TACTL |= TASSEL_2;     //Internal osc
	TACTL |= ID_0;         //1x prescaler
	change_bit(TACTL,1,on) //change the mode, on=1 => count up to taccr0, on=0 => stop
	cicad_calculate_period(0);
}