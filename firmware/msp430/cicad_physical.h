#ifndef CICAD_PHYSICAL_H
#define CICAD_PHYSICAL_H

///expects CICAD_PIN to be defined.
///@TODO Have a way to input the MCU port into this file
#include "config.h"

///Dominant bit definition, if it's either a logical low or high.
///@note Polarity 0 belived to work.
#define CICAD_POLARITY 1

///Defines for easy use.
///@note these are not the logic level. But LOGIC_LEVEL_FOR_DOMINANT = CICAD_DOMINANT XNOR CICAD_POLARITY.
///@example cicad_send_bit(CICAD_DOMINANT); //Send SoM
#define CICAD_DOMINANT 1
#define CICAD_RECESSIVE 0

///Setups the pins for the physical layer.
void cicad_physical_init();

///Reads a bit.
///@returns 1 on dominant bit.
///@returns 0 on recessive.
///@note there is no delay in this function.
inline unsigned char cicad_read_bit();

///Sends a bit.
///@param val a 1 here will make the bus go to dominant, 0 will change it back to high Z so it goes back to recessive.
///@note there is no delay in this function.
inline void cicad_send_bit(unsigned char val);

///Setups the timer.
///@param on if the timer should be on or off.
void cicad_init_timer(unsigned char on);

///Periods between bits, stored and calculated by cicad_calculate_period to save time..
volatile unsigned int cicad_1_period, cicad_1_5_period;

///Sets the cicad_1_period, calculates the cicad_1_5_period to save time.
void cicad_set_period(unsigned int period);

///Makes it so the timer counts to val.
///@param val Sets TACCR0 to val. cicad_1_period, cicad_1_5_period can be used in here.
#define CICAD_SET_TIMER(val) (TACCR0=val);

///@returns nonzero when time period has passed.
///@note CICAD_RESET_CHECK_TIME should be used after this returns 1 for the next period to work correctly.
#define CICAD_CHECK_TIME (TACCTL0 & CCIFG)

///Resets the CHECK_TIME flag.
#define CICAD_RESET_CHECK_TIME (TACCTL0&=~CCIFG)

///Combination of CICAD_CHECK_TIME and CICAD_RESET_CHECK_TIME
#define CICAD_WAIT_NEXT_BIT while(!CICAD_CHECK_TIME); CICAD_RESET_CHECK_TIME;

///Resets the timer to start from 0 again.
#define CICAD_TIMER_RESET (TAR=0)

#endif