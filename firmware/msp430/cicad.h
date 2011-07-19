#ifndef CICAD_H
#define CICAD_H

#include "cicad_physical.h"

///Status defines
#define OK 0
#define PIORITY_LOST 1
#define CANNOT_RECESS 1
#define NO_ACK_RECIEVED 2

///Sends one byte
///@returns one of: OK CANNOT_RECESS
///@TODO this function should return CANNOT_RECESS when it detects a dominant when outputting recessive
inline unsigned char cicad_send_byte(unsigned char byte);

///Sends one cicad message
///@param id Message ID. Lower Will always take priority. If most significant bit is dominant, it means the current byte is not the last one. This goes up to 3 bytes.
///@param n Length of the message. 4 bits of length allows a maximum of 15 bytes of data.
///@param message Message contents, up to 15 bytes long.
///@returns one of: OK PIORITY_LOST NO_ACK_RECIEVED
///@TODO Implement PIORITY_LOST, probably with a macro wrapper around cicad_send_byte
unsigned char cicad_send_message(unsigned long id, unsigned char n, unsigned char *message);

#endif