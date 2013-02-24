#include "bitop.h"
#include "debug.h"

#include "cicad_physical.h"

void cicad_physical_init() {
	clear_bit(P1DIR, CICAD_PIN); cicad_sending_bit=0; //Input/Recessive
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
	cicad_sending_bit=val; //Remember what we're sending
}

void cicad_calculate_period(unsigned int period) {
	cicad_1_period=period;
	cicad_0_5_period=period/2;
	cicad_1_5_period=cicad_1_period+cicad_0_5_period;
}

void cicad_init_timer(bool on) {
	TACTL |= TASSEL_2;      //Internal osc
	TACTL |= ID_0;          //1x prescaler
	change_bit(TACTL,4,on); //Change the mode, on=true => count up to taccr0, on=false => stop
	
	//Enable Interrupts
	CCTL0 |= CCIE; //Timer
	_BIS_SR(GIE); //Global
	
	CICAD_TIMER_RESET;      //Start the counter from 0
}