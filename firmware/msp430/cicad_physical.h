#ifndef CICAD_PHYSICAL_H
#define CICAD_PHYSICAL_H

///expects CICAD_PIN to be defined
#include "config.h"

void cicad_init();

inline unsigned char cicad_read_bit();

inline void cicad_send_bit(unsigned char val);

#endif