/*
 * cicad-bus.c
 *
 *  Created on: Jan 21, 2011
 *      Author: dstan
 *
 *  Implement the controller inter communication array databus
 */

////////////////////////////////////////////////////////////////////////////

//states
#define ST_IDLE 0
#define ST_SOM	1
#define ST_

#define OK  1
#define NOK 0
#define TIMER_OVERFLOW (TIFR0&(1<<OCF0A))

void USART_Transmit(unsigned char byte);
////////////////////////////////////////////////////////////////////////////
const unsigned char mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char u8_rec_buff[22];
unsigned char u8_rec_data_len=0;
/*unsigned char index_byte = 0;
unsigned char received_bit_index=0;*/

unsigned int u16_crc=0x0000;
#define CRC_POLY 0xC86C
#define BIT_TIME 10 // 10 - 50KHz; 20 - 25KHz
unsigned char u8_trans_buff[22];
//unsigned char u8_trans_bytes = 0;


inline void check_crc(unsigned char u8_rec_bit)
//u8rec_bit must be 0 or 1
// cecks crc one bit a time
{
	//if 1 reached MSB then shift left, add the new bit, and do the XOR with the crc polynome; else just shift left and add the new bit;
	if(u16_crc & 0x8000)
	{
		u16_crc<<=1;
		u16_crc |= (unsigned int)u8_rec_bit;
		u16_crc ^= CRC_POLY;
	}
	else
	{
		u16_crc<<=1;
		u16_crc |= (unsigned int)u8_rec_bit;
	}
}


unsigned char hist=1; // init so it won't trigger first time

inline unsigned char read_bit_sync_count(void)
// waits 10us until mesures the line state and returns
// it performs a sync on recessive->dominant transition - on detection waits 5us to return
// returns 1 if transition detected and returns immediate upon detection
// waits the _us time and returns 0 if no transition detected
{
	unsigned char temp=0;

	// wait until timer expires or 0->1 transition

	while(!TIMER_OVERFLOW) // wait for it
	{
		temp=IR_REC_RELEASED;
		if(temp && !hist)
		{
			TIFR0 |= (1<<OCF0A); //reset the flag
			TCNT0 = 0; //reset timer
			OCR0A = 95; //value for sync

			hist=temp;
			return 1;
		}
		hist=temp;
	}
	TIFR0 |= (1<<OCF0A); //reset the flag
	OCR0A = 81; //value for 10us
	//CLEAR_LED_G;CLEAR_LED_G;SET_LED_G;SET_LED_G;

	return temp;
}


inline unsigned char receive_byte(void)
{
	// receive 8 bits
	unsigned char byte_value=0;
	unsigned char index=0;

	for(index=0; index<8; index++)
	{
		if(read_bit_sync_count())
		{
			byte_value |= mask[index];
			check_crc(1);
		}
		else
		{
			check_crc(0);
		}
	}
	read_bit_sync_count(); // read and sync to the delimiter
	return byte_value;
}

inline void cicad_receive(void)
{
	unsigned char index;
	unsigned char temp;

	// set and start the timer
	TIFR0 |= (1<<OCF0A); //reset the flag
	TCCR0A = (1<<WGM01); // CTC mode
	TCNT0 = 0; //reset timer
	OCR0A = 43; // first value to account on isr lag
	TCCR0B = 0x01; // clock and start

	u16_crc=0;
	//received recessive->dominant transition
	//read_bit_sync_count(4);
	//receive 6 bytes
	u8_rec_buff[0]=receive_byte(); // ID byte 1
	if((u8_rec_buff[0] & 0x80)) u8_rec_buff[1]=receive_byte(); // ID byte 2
	if((u8_rec_buff[1] & 0x80)) u8_rec_buff[2]=receive_byte(); // ID byte 3

	u8_rec_buff[3]=receive_byte(); // CTRL byte
	temp = (u8_rec_buff[3] & 0x0F)+4 +2; // 2bytes CRC
	// Data + CRC bytes - because CRC is calculated on the fly, we don't really need them into the buffer, but to be received.
	for(index=4;index<temp;index++)
	{
		//put bytes in the buffer
		u8_rec_buff[index]=receive_byte();

	}
	u8_rec_data_len = (u8_rec_buff[3] & 0x0F);
	hist=1;
	//TODO ack should come here - do it when HW is ready
}

inline void function_SIG_PIN_CHANGE0(void)
{
	unsigned char index;
	// input pin toggled
	if(IR_REC_RELEASED) // raised
	{
		cicad_receive();
		TCCR0B = 0x00; //stop timer
		// send buffer to uart - just for test, it spends a lot of time here
				for(index=0;index<u8_rec_data_len+6;index++)
				{
					USART_Transmit(u8_rec_buff[index]);
				}
				if(u16_crc!=0) USART_Transmit(0xFF);
	}


}
ISR (SIG_PIN_CHANGE0 ){ function_SIG_PIN_CHANGE0(); }


inline unsigned char send_cicad_byte(unsigned char byte)
{
#define T_VAL_1 38
#define T_VAL_2 37
	unsigned char i=0;

	for(i=0;i<8;i++)
	{
		//wait end of previous byte
		while(!TIMER_OVERFLOW);
		// set the pin
		if(byte & mask[i]) SET_IR_EM;
		else CLEAR_IR_EM;
		TIFR0 |= (1<<OCF0A); //reset the flag
		OCR0A = T_VAL_1; //value for 5us
		while(!TIMER_OVERFLOW); // wait expiration
		// TODO check if no colision and no priority lost - when HW is done
		TIFR0 |= (1<<OCF0A); //reset the flag
		OCR0A = T_VAL_2; //value for 5us
	}
	while(!TIMER_OVERFLOW); // wait expiration
	//send delimiter
	if(byte & 0x01) CLEAR_IR_EM;
	else SET_IR_EM;
	TIFR0 |= (1<<OCF0A); //reset the flag
	OCR0A = T_VAL_1; //value for 5us
	while(!TIMER_OVERFLOW); // wait expiration
	// TODO check if no colision and no priority lost - when HW is done
	TIFR0 |= (1<<OCF0A); //reset the flag
	OCR0A = T_VAL_2; //value for 5us
	return OK;
}

unsigned char send_cicad_message(unsigned char len_bytes)
// returns OK if message sent ok;
// returns NOK if priority lost;
{
	unsigned char index_byte;
	// TODO wait until no message on comm media; implement wait for bus free on transmitting - it needs a kind of global status

	// set and start the timer
	TIFR0 |= (1<<OCF0A); //reset the flag
	TCCR0A = (1<<WGM01); // CTC mode
	TCNT0 = 0; //reset timer
	OCR0A = 65; // first value to account on isr delay
	TCCR0B = 0x01; // clock and start
	// START OF FRAME
	SET_IR_EM;

	for(index_byte=0;index_byte<len_bytes;index_byte++)
	{
		if(send_cicad_byte(u8_trans_buff[index_byte])!= OK) return NOK;
	}
	//send ACK delimiter - send recessive
	while(!TIMER_OVERFLOW); // wait expiration
	TIFR0 |= (1<<OCF0A); //reset the flag
	CLEAR_IR_EM;
	while(!TIMER_OVERFLOW); // wait expiration
	TIFR0 |= (1<<OCF0A); //reset the flag
	//wait ACK here TODO
	TCCR0B = 0x00; //stop timer
	return OK;
}





void calculate_crc(unsigned char len_bytes)
// calculates crc for a specific message in the u8_trans_buff;
// the len_bytes is the len of the message with crc bytes as 0x00; it replaces the crc into the u8_trans_buff;
{
	unsigned char index_bit;
	unsigned char index_byte;
	u16_crc = 0;
	for(index_byte=0;index_byte<len_bytes;index_byte++)
		for(index_bit=8;index_bit>0;index_bit--)
			if(u8_trans_buff[index_byte] & (1<<(index_bit-1))) check_crc(0x01);
			else check_crc(0x00);
	// replace 0x00's with the calculated crc
	u8_trans_buff[len_bytes-2] = (unsigned char)(u16_crc>>8);
	u8_trans_buff[len_bytes-1] = (unsigned char)(u16_crc & 0x00FF);
}



////////////////////////////////////////////////////////////////////////////
