#ifndef CICAD_H
#define CICAD_H

#include "cicad_physical.h"

///Initializes the openCICAD library
///@param cycles How many cpu cycles is a clock pulse for the protocol. 0x640 cycles = 100us on a 16Mhz cpu with 1x prescaler.
void cicad_init(unsigned int cycles);

///Status defines.
///@todo Implement a way to notify these.
#define CICAD_OK 0
#define CICAD_PIORITY_LOST 1
#define CICAD_NO_ACK_RECIEVED 2

///Used for storing the state of the transmission/recieving process because the execution is going to be split throughout interrupts.
volatile enum State {
	IDLE,   ///<Idle state, when there's no transmission in progress.
	SOM,    ///<Start of message bit
	BUFFER, ///<The bytes that use the buffer: Message Type, Destination Address, Control(Flags, Length), Data, CRC
	ACK,    ///<Acknowlege from someone else that they recieved the packet
	EOM,    ///<End of message bit
} cicad_state;

///True while transmitting. Used by the timer interrupt.
bool transmit;

///Sends one cicad message
///@param id Message ID. Higher will always take priority.
///@param n Length of the message. 4 bits of length allows a maximum of 15 bytes of data.
///@param message Message contents, up to 15 bytes long.
void cicad_send_message(unsigned long id, unsigned long address, unsigned char n, unsigned char *message);

#endif
