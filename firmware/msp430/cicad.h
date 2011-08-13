#ifndef CICAD_H
#define CICAD_H

#include "cicad_physical.h"

///Status defines
#define CICAD_OK 0
#define CICAD_PIORITY_LOST 1
#define CICAD_CANNOT_RECESS 1
#define CICAD_NO_ACK_RECIEVED 2

///Sends one cicad message
///@param id Message ID. Lower Will always take priority. If most significant bit is dominant, it means the current byte is not the last one. This goes up to 3 bytes.
///@param n Length of the message. 4 bits of length allows a maximum of 15 bytes of data.
///@param message Message contents, up to 15 bytes long.
///@returns one of: CICAD_OK CICAD_PIORITY_LOST CICAD_NO_ACK_RECIEVED
///@TODO Implement CICAD_PIORITY_LOST, probably with a macro wrapper around cicad_send_byte
unsigned char cicad_send_message(unsigned long id, unsigned char n, unsigned char *message);

#endif