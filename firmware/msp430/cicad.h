#ifndef CICAD_H
#define CICAD_H

#include "cicad_physical.h"

///Status defines
#define OK 0
#define PIORITY_LOST 1
#define NO_ACK_RECIEVED 2

///Sends one byte
///@TODO this function should return error when it detects a dominant when outputting recessive
inline void cicad_send_byte(unsigned char byte);

///Sends one cicad message
///@param id Message ID. Lower Will always take priority. If most significant bit is dominant, it means the current byte is not the last one. This goes up to 3 bytes.
///@param n Length of the message. 4 bits of length allows a maximum of 15 bytes of data.
///@param message Message contents, up to 15 bytes long.
///@returns one of: OK PIORITY_LOST NO_ACK_RECIEVED
unsigned char cicad_send_message(unsigned long id, unsigned char n, unsigned char *message);

#endif