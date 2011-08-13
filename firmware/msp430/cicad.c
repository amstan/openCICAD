#include "bitop.h"
#include "debug.h"

#include "cicad.h"

inline unsigned char cicad_send_byte(unsigned char byte) {
	char i;
	unsigned char b;
	
	CICAD_SET_TIMER(cicad_0_5_period);
	
	//one by one send a bit from the byte
	for(i=7; i>=0; i--) {
		b=test_bit(byte,i);
		CICAD_WAIT_NEXT_BIT;
		cicad_send_bit(b);
	}
	
	//send sync bit, inverse of the last one
	CICAD_WAIT_NEXT_BIT;
	cicad_send_bit(!b);
	
	return CICAD_OK;
}

unsigned char cicad_send_message(unsigned long id, unsigned char n, unsigned char *message) {
	unsigned char i;
	
	//Initialize the timer
	CICAD_SET_TIMER(cicad_1_period);
	cicad_init_timer(1);
	
	///@TODO Wait till the line is clear
	
	//Start of Message
	cicad_send_bit(CICAD_DOMINANT);
	
	//ID
	unsigned char id2, id1, id0;
	#define MORE_ID 0b1000000;
	id0=id; id<<=8;
	if(id>0) {
		id1=id & MORE_ID; id<<=7;
		if(id>0) {
			id2=id & MORE_ID;
			cicad_send_byte(id2);
		}
		cicad_send_byte(id1);
	}
	cicad_send_byte(id0);
	
	//Control bits + Length
	unsigned char ctrl;
	ctrl=n;
	cicad_send_byte(ctrl);
	
	//Data
	for(i=0;i<n;i++) {
		cicad_send_byte(message[i]);
	}
	
	///@TODO CRC HERE
	
	///@TODO ACK HERE
	
	//End of Message
	for(i=0;i<10;i++) {
		CICAD_WAIT_NEXT_BIT;
		cicad_send_bit(CICAD_RECESSIVE);
	}
	
	CICAD_WAIT_NEXT_BIT;
	cicad_init_timer(0);
	
	return CICAD_OK;
}