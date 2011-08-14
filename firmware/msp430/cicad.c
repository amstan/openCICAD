#include "bitop.h"
#include "debug.h"

#include "cicad.h"

void cicad_init(unsigned int cycles) {
	cicad_physical_init();
	cicad_state=clear;
	cicad_calculate_period(cycles);
}

///End of Message Length, this is 10 because it's one bit longer than a byte+delimiter.
#define CICAD_EOM_LENGTH 10

///Buffer used in rx/tx.
volatile unsigned char buffer[3/*id*/+1/*ctrl*/+15/*data*/+2/*crc*/];

///Length of buffer.
volatile unsigned char nb;

///The element in the buffer where the interrupts continue reading from.
volatile unsigned char ib;

///The bit in the buffer[ib] byte where the interrupts continue reading from.
volatile char ibit;

///Appends a byte to the buffer.
#define ADD_BUFFER(byte) buffer[nb++]=byte;

#define state cicad_state

///Used in txing to differentiate the interrupt when there's a change in a signal vs the interrupt where the priority check is done.
unsigned char halfperiod;

///Used to count the end of message periods.
unsigned char eom_count = 0;

///@todo Rename this function, should be internal use only. This function should not block, and should have a wrapper around it that does cool things.
void cicad_send_message(unsigned long id, unsigned char n, unsigned char *message) {
	unsigned char i;
	
	//Wait till the line is clear
	///@todo Make this nonblocking
	while(state!=clear);
	
	//Start transmitting the start of message
	state=som;
	
	//init the tx states
	ibit=7;
	transmit=1;
	halfperiod=0;
	
	//start the timer
	CICAD_SET_TIMER(cicad_0_5_period);
	cicad_init_timer(1);
	
	//Initialize the buffer
	nb=0;
	ib=0;
	
	//ID
	unsigned char id2, id1, id0;
	#define MORE_ID 0b1000000;
	id0=id; id>>=8;
	if(id>0) {
		id1=id | MORE_ID; id>>=7;
		if(id>0) {
			id2=id | MORE_ID;
			ADD_BUFFER(id2);
		}
		ADD_BUFFER(id1);
	}
	ADD_BUFFER(id0);
	
	//Control bits + Length
	unsigned char ctrl;
	ctrl=n;
	ADD_BUFFER(ctrl);

	//Data
	for(i=0;i<n;i++) {
		ADD_BUFFER(message[i]);
	}
	
	///@todo CRC HERE
}

///Transmits the a cicad message using the tx states and the buffer. This function is called by the cicad_timer_vector interrupt.
void cicad_transmit(void) {
	if(!halfperiod) {
		//Point where the signal should change
		switch(state) {
			case som:
				cicad_send_bit(1);
				break;
			
			case frombuffer:
				//Output the proper bit
				if(ibit>=0) {
					cicad_send_bit(test_bit(buffer[ib],ibit));
					ibit--;
				} else {
					//Output the byte limiter
					cicad_send_bit(cicad_sending_bit^1); //byte delimiter is just the last bit inverted
					ibit=7;
				}
				
				//Increment the byte index
				if(ibit==7) {
					ib++;
				}
				
				break;
			
			case ack:
				//Quiet so we can listen to incoming
				cicad_send_bit(0);
				
				//see the halfperiod branch for more
				break;
			
			case eom:
				cicad_send_bit(0); //Send recessive bits
				
				//Repeat CICAD_EOM_LENGTH times
				eom_count++;
				eom_count%=CICAD_EOM_LENGTH;
				
				//See other middle signal branch for the stop condition
				
				break;
	}
	} else {
		//Middle of the signal transmission
		
		//Try to only increase state in here
		switch(state) {
			case som:
				//Bus will be high here
				state++;
				break;
			
			case frombuffer:
				//This is while transmitting bit from buffer
				
				//Check if a recessive bit is actually recessive or someone else is transmitting
				if(cicad_sending_bit!=cicad_read_bit()) {
					//Loss of priority here
					
					///@todo add priority loss message instead of turning the led on.
					set_bit(P1OUT,LED);
					//CICAD_PIORITY_LOST
				}
				
				//Check if end of buffer
				if (ib==nb) {
					state++;
				}
				
				break;
			
			case ack:
				//Check for ack
				
				if(cicad_read_bit()!=1) {
					///@todo Something when there's no ack on tx
					//CICAD_NO_ACK_RECIEVED
				}
				
				state++;
				
			break;
			
			case eom:
				//There should be nobody transmitting in here
				
				//Check if we have to stop the eom
				if(eom_count==0) {
					state=clear;
					cicad_init_timer(0); //Stop the timer
				}
				
				break;
		}
	}
	
	halfperiod^=1;
}

CICAD_TIMER_INTERRUPT_VECTOR cicad_timer_interrupt() {
	if(transmit) {
		cicad_transmit();
	} else {
		///@todo Recieve stuff
	}
}