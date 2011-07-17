////////////////////		Dusan Stan		////////////////////////////////
/////////	Main file for 1WC		////////////////////////////


#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>

#include <delay12.h>	//Modified Delay for 12MHZ clk
#include <bitop.h>		//BIT operations macro
#include <port.h>		//Port names assignment and eeprom512K

#include <avr/interrupt.h>
//#include <avr/signal.h>

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <openCICAD.c>
////////////////////////////////////////////////////////////////////////////
#include <avr/eeprom.h> //Internal EEprom
#define EEPROM_SECTION  __attribute__ ((section (".eeprom")))

////////////////////////////////////////////////////////////////////////////

#define OFF	0
#define ON	1

#define FALSE 0
#define TRUE 1

#define BAUD	1	// 40 = 2400; 19 = 4800; 9 = 9600; 4 = 19200; 1 = 38400 @ 8MHz
//#define OUT_DEBUG_INFO  // for debug board only

////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
/*unsigned char UART_parity(unsigned char recData)
{
	unsigned char bitsum, i;
	bitsum = 0;
	for (i =0; i<8; i++)
	{
		bitsum = bitsum + ( (recData >> i) & 0x01 );
	}
	return bitsum & 0x01;

}*/

////////////////////////////////////////////////////////////////////////////
void USART_Transmit(unsigned char byte)
{
	// implemented in software, Tiny24 doesn't have uart
	// 8MHz internal osc, division by 8
	// 8 bit data, no parity, 1 stop

	volatile unsigned char i;
	volatile unsigned char j;

	//DDRB= DDRB | 0b00000100;	// PB2  as output
	_delay_us(1);
	// start
	SET_TX;
	for(j=0;j<10;j++)
		_delay_us(BAUD);

	// 8 bit data
	for(i=0;i<8;i++)
	{
		if( (byte>>i) & 0x01 )
		{
			CLEAR_TX;
		}
		else
		{
			SET_TX;
		}
		for(j=0;j<10;j++)
		_delay_us(BAUD);
	}
	// parity
	/*if(UART_parity(byte))//even parity
	{
		CLEAR_TX;
	}
	else
	{
		SET_TX;
	}
	for(j=0;j<10;j++)
		_delay_us(BAUD);*/

	// stop
	CLEAR_TX;
	for(j=0;j<10;j++)
		_delay_us(BAUD);
	//DDRB= DDRB & 0b11111011;	// PB2  as input
}

////////////////////////////////////////////////////////////////////////////
void mydelay(unsigned char time2ms)
{
	unsigned char index;

	for(index=0;index<time2ms;index++)
	{
		_delay_ms(2);
	}
}






int main(void)
{

	unsigned char i=0;

	 wdt_disable();


    // Initializations of port registers

	DDRB=0b00000011;	//
	//PORTB=0b00000001;	// PB0 pull-up
	DDRA=0b01111010;	// 1.3.4.5.6 as output
	//PORTA=0b01001000;	// PA3, PA6 pull-up


	// set up HW registers for pin change interrupt INT0 rising edge
	//MCUCR |= 0x03;
	//GIMSK = (1<<INT0);
    PCMSK0 = (1<<PCINT0);
	//PCMSK1 = (1<<PCINT10);
	GIMSK = (1<<PCIE0);// | (1<<PCIE1);

	//cli(); // disable interrupts
    sei(); // enable interrupts

	//build message
	u8_trans_buff[0]=0xAA; // ID1
	u8_trans_buff[1]=0xAA; // ID2
	u8_trans_buff[2]=0x55; // ID3
	u8_trans_buff[3]=0x0F; // controll byte, 15 bytes data
	u8_trans_buff[4]=0x00; // data
	u8_trans_buff[5]=0x02;
	u8_trans_buff[6]=0xFF;
	u8_trans_buff[7]=0xAA;
	u8_trans_buff[8]=0x55;
	u8_trans_buff[9]=0x00;
	u8_trans_buff[10]=0x7;
	u8_trans_buff[11]=0x8;
	u8_trans_buff[12]=0xFF;
	u8_trans_buff[13]=0x0A;
	u8_trans_buff[14]=0x0B;
	u8_trans_buff[15]=0xAA;
	u8_trans_buff[16]=0x00;
	u8_trans_buff[17]=0xFF;
	u8_trans_buff[18]=0x0F;

	while(1)
	{

//#define TRANS

#ifdef TRANS	//transmitter
		mydelay(40);

/*		//build message
		u8_trans_buff[0]=0x80; // ID1
		u8_trans_buff[1]=0xFF; // ID2
		u8_trans_buff[2]=0x00; // ID3
		u8_trans_buff[3]=0x02; // controll byte, 2 bytes data
		u8_trans_buff[4]=0x00; // data
		u8_trans_buff[5]=0xFF;
		u8_trans_buff[6]=0x00;// crc1 place
		u8_trans_buff[7]=0x00; // crc2 place

		calculate_crc(8); // calculates the crc and puts it in the buffer

		//send
		send_cicad_message(8);*/


		u8_trans_buff[19]=0x00; // crc1 place
		u8_trans_buff[20]=0x00; // crc2 place
		calculate_crc(21); // calculates the crc and puts it in the buffer

		//send
		send_cicad_message(21);
		mydelay(50);
		send_cicad_message(21);

		for(i=4;i<19;i++) u8_trans_buff[i]++;


#else	//receiver

		// nothing to do, receive is isr

#endif
	}
	//Code should not reach here
	return 1;
}//End of code

////////////////////////////////////////////////////////////////////////////
