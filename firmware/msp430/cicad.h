#ifndef CICAD_H
#define CICAD_H

#include "cicad_physical.h"

///Initializes the openCicad library
///@param cycles How many cpu cycles is a clock pulse for the protocol. 0x640 cycles = 100us on a 16Mhz cpu with 1x prescaler.
void cicad_init(unsigned int cycles);

///Status defines.
///@todo Implement a way to notify these.
#define CICAD_OK 0
#define CICAD_PIORITY_LOST 1
#define CICAD_NO_ACK_RECIEVED 2

///State for the tx/rx.
volatile enum State {
	clear,
	som,
	
	///The stuff to get from the buffer: id, ctrl, data, crc
	frombuffer,
	
	ack,
	eom,
} cicad_state;

///True while transmitting. Used by the timer interrupt.
unsigned char transmit;

///Sends one cicad message
///@param id Message ID. Lower Will always take priority. If most significant bit is dominant, it means the current byte is not the last one. This goes up to 3 bytes.
///@param n Length of the message. 4 bits of length allows a maximum of 15 bytes of data.
///@param message Message contents, up to 15 bytes long.
void cicad_send_message(unsigned long id, unsigned char n, unsigned char *message);

#endif
