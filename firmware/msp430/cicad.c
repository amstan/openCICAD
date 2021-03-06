#include "bitop.h"
#include "debug.h"

#include "cicad.h"

void cicad_init(unsigned int cycles) {
	cicad_physical_init();
	cicad_state=IDLE;
	cicad_calculate_period(cycles);
}

///End of Message Length, this is 10 because it's one bit longer than a byte+delimiter.
#define CICAD_EOM_LENGTH 10

///Buffer used in rx/tx.
volatile unsigned char buffer[2/*message type*/+3/*destination address*/+1/*control byte*/+15/*data*/+2/*crc*/];

///Length of buffer.
volatile unsigned char nbuffer;

///The element in the buffer where the interrupts continue reading from.
volatile unsigned char ibuffer;

///The bit in the buffer[ibuffer] byte where the interrupts continue reading from.
volatile signed char ibit;

///Appends a byte to the buffer.
#define ADD_BUFFER(byte) buffer[nbuffer++]=byte;

#define state cicad_state

///Used in txing to differentiate the interrupt when there's a change in a signal vs the interrupt where the priority check is done.
bool halfperiod;

///Used to count the end of message periods, goes up to CICAD_EOM_LENGTH
unsigned char eom_count = 0;

///@todo Rename this function, should be internal use only. This function should not block, and should have a wrapper around it that does cool things.
void cicad_send_message(unsigned long id, unsigned long address, unsigned char n, unsigned char *message) {
	unsigned char i;
	
	//Wait till the line is IDLE
	///@todo Make this nonblocking
	while(state!=IDLE);
	
	//Start transmitting the start of message
	state=SOM;
	
	//init the tx states
	ibit=7;
	transmit=true;
	halfperiod=false;
	
	//Initialize the buffer
	nbuffer=0;
	ibuffer=0;
	
	//start the timer
	CICAD_SET_TIMER(cicad_0_5_period);
	cicad_init_timer(true);
	
	//ID
	ADD_BUFFER(id/8);
	ADD_BUFFER(id%8);
	
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

///Transmits a cicad message half a bit at a time. This function is called by the cicad_timer_vector interrupt.
void cicad_transmit(void) {
	if(!halfperiod) {
		//Point where the signal should change
		switch(state) {
			case SOM:
				cicad_send_bit(CICAD_DOMINANT);
				break;
			
			case BUFFER:
				//Output the proper bit
				if(ibit>=0) {
					cicad_send_bit(test_bit(buffer[ibuffer],ibit));
					ibit--;
				} else {
					//Output the byte limiter
					cicad_send_bit(!cicad_sending_bit); //byte delimiter is just the last bit inverted
					
					//Go to next byte
					ibit=7;
					ibuffer++;
				}
				break;
			
			case ACK:
				//Quiet so we can listen to incoming
				cicad_send_bit(0);
				
				//see the halfperiod branch for more
				break;
			
			case EOM:
				cicad_send_bit(0); //Send recessive bits
				
				//Repeat CICAD_EOM_LENGTH times
				eom_count++;
				eom_count%=CICAD_EOM_LENGTH;
				
				//See other middle signal branch for the stop condition
				
				break;
			
			case IDLE:
				///@todo this should never happen
				break;
	}
	} else {
		//Middle of the signal transmission
		
		//Try to only increase state in here
		switch(state) {
			case SOM:
				//Bus will be high here
				state++;
				break;
			
			case BUFFER:
				//This is while transmitting bit from buffer
				
				//Check if a recessive bit is actually recessive or someone else is transmitting
				if(cicad_sending_bit!=cicad_read_bit()) {
					//Loss of priority here
					
					///@todo add priority loss message instead of turning the led on.
					set_bit(P1OUT,LED);
					//CICAD_PIORITY_LOST
				}
				
				//Check if end of buffer
				if (ibuffer==nbuffer) {
					state++;
				}
				
				break;
			
			case ACK:
				//Check for ack
				
				if(cicad_read_bit()!=CICAD_DOMINANT) {
					///@todo Something when there's no ack on tx
					//CICAD_NO_ACK_RECIEVED
				}
				
				state++;
				
			break;
			
			case EOM:
				//There should be nobody transmitting in here
				
				//Check if we have to stop the eom
				if(eom_count==0) {
					state=IDLE;
					cicad_init_timer(0); //Stop the timer
				}
				
				break;
			
			case IDLE:
				///@todo this should never happen
				break;
		}
	}
	
	halfperiod^=1; //flip this for next interrupt
}

CICAD_TIMER_INTERRUPT_VECTOR cicad_timer_interrupt() {
	if(transmit) {
		cicad_transmit();
	} else {
		///@todo Recieve stuff
		
		//Pulse on a read
		unsigned char t;
		t=cicad_read_bit();
		set_bit(P1OUT,LED);
		__delay_cycles(100);
		if(t)
			__delay_cycles(300);
		clear_bit(P1OUT,LED);
	}
}