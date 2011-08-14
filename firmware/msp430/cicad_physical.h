#ifndef CICAD_PHYSICAL_H
#define CICAD_PHYSICAL_H


///@file cicad_physical.h
///@note This file includes config.h and it expects CICAD_PIN to be defined.
///@todo Have a way to input the MCU port from config.h to openCICAD.
#include "config.h"

///Dominant bit definition, if it's either a logical low or high.
///@note Polarity 0 belived to work.
#define CICAD_POLARITY 1

///Defines for easy use.
///@note these are not the logic level. But LOGIC_LEVEL_FOR_DOMINANT = CICAD_DOMINANT XNOR CICAD_POLARITY.
#define CICAD_DOMINANT 1
#define CICAD_RECESSIVE 0

///Setups the pins for the physical layer.
void cicad_physical_init();

///Reads a bit.
///@returns 1 on dominant bit.
///@returns 0 on recessive.
///@note there is no delay in this function.
inline unsigned char cicad_read_bit();

///The last bit sent with cicad_send_bit
///@note This bit can be used to discover if lost priority
unsigned char cicad_sending_bit;

///Sends a bit.
///@param val a 1 here will make the bus go to dominant, 0 will change it back to high Z so it goes back to recessive.
///@note there is no delay in this function.
inline void cicad_send_bit(unsigned char val);

///Setups the timer.
///@param on if the timer should be on or off.
///@note This function also clears the overflow flag and the value in the timer.
///@note This function enables the timer interrupt.
void cicad_init_timer(unsigned char on);

///Interrupt vector function call macro
#define CICAD_TIMER_INTERRUPT_VECTOR void __attribute__((interrupt (TIMER0_A0_VECTOR)))

///Periods between bits, stored and calculated by cicad_calculate_period to save time.
volatile unsigned int cicad_0_5_period, cicad_1_period, cicad_1_5_period;

///Period of the data, the smaller this is the faster the bus speed. Period measured in cpu cycles.
///This sets the cicad_1_period, calculates the cicad_0_5_period and cicad_1_5_period to save time.
void cicad_calculate_period(unsigned int period);

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

///Resets the timer to start from 0 again. Also clears the CHECK_TIME flag.
#define CICAD_TIMER_RESET (TAR=0); CICAD_RESET_CHECK_TIME;

///Moves the timer to x, sets TAR=x.
///@note This is used to the syncing of the timer to the line when recieving.
#define CICAD_TIMER_MOVE(x) TAR=x; CICAD_RESET_CHECK_TIME;

#endif