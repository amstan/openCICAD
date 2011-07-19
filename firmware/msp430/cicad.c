#include "bitop.h"
#include "debug.h"

#include "cicad.h"

inline void cicad_send_byte(unsigned char byte) {
	char i;
	unsigned char b;
	
	//one by one send a bit from the byte
	for(i=7; i>=0; i--) {
		b=test_bit(byte,i);
		CICAD_WAIT_NEXT_BIT;
		cicad_send_bit(b);
	}
	
	//send sync bit, inverse of the last one
	CICAD_WAIT_NEXT_BIT;
	cicad_send_bit(~b);
}


unsigned char cicad_send_message(unsigned long id, unsigned char n, unsigned char *message) {
	///@TODO Wait till the line is clear 
	
	//SoM
	cicad_send_bit(1);
	
	//ID
	#define MORE_ID 0b1000000;
	unsigned char id2, id1, id0;
	id0=id; id<<=8;
	if(id>0) {
		id1=id & MORE_ID; id<<=7;
		if(id>0) {
			id2=id & MORE_ID;
		}
	}
	if(id2>0) {
		CICAD_WAIT_NEXT_BIT;
		cicad_send_bit(id2);
	}
	
	return 0;
}