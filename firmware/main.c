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

/*
// legacy from HT
#define MotorOnTime   800           //800*4ms=3.2S
#define HANDOFF_TIMEOUT	10	//it was 10 - 2 second
#define SET_IN 0b00000010
#define IR_REC 0b00000000
#define V_REF 0b00100001

#define LED_RED		1
#define LED_GREEN	2

#define ConLedON  0
#define ConLedOFF 1

#define MODE_START			0
#define MODE_IDLE			1
#define MODE_MOTOR_ON  		2
#define MODE_SAMPLE			3
#define MODE_HANDSOFF		4

#define FMODE_VERSION_FLASH	4
#define FMODE_HANDOFFEND	3
#define FMODE_HANDOFF		2
#define FMODE_LOWBAT		1
#define FMODE_READY			0
*/

#define BAUD	1	// 40 = 2400; 19 = 4800; 9 = 9600; 4 = 19200; 1 = 38400 @ 8MHz
//#define OUT_DEBUG_INFO  // for debug board only

////////////////////////////////////////////////////////////////////////////


/*unsigned long 	Ticks;					//Tmr1 overflows every 4 ms
unsigned char HandOffTimeOutCounter=0;
unsigned char	preState;*/


////////////////////////////////////////////////////////////////////////////
/*ISR(SIG_WATCHDOG_TIMEOUT)
{
	// just reset the bit
	// set up the watchdog for wake up
	WDTCSR |= (1<<WDIE);
}*/

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
	// 2400, 8 bit data, no parity, 1 stop

	volatile unsigned char i;
	volatile unsigned char j;

	//DDRB= DDRB | 0b00000100;	// PB2  as output
	_delay_us(20);
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

////////////////////////////////////////////////////////////////////////////
/*void disableADC(void)
{
	// power down ADC
	ADCSRA = 0;
}*/

////////////////////////////////////////////////////////////////////////////
/*unsigned char measADC(unsigned char channel)
{
	//left adj
	ADCSRB |= (1<<ADLAR);
	ADMUX = channel;
	_delay_us(80);

	ADCSRA=0xC0;
	//wait conversion end
	while(!(ADCSRA & 0x10));
	//TODO (1<<ADIF) ; // clear the bit - modify the file so it reads only once the aADC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// power down ADC
	//ADCSRA = 0;
	return ADCH;
}*/



/*
unsigned char abso(unsigned char num1,unsigned char num2)
{
	if (num1>=num2) return (num1-num2);
	else return (num2-num1);
}//Return absolute difference
*/


//#define TRANS

int main(void)
{



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

    // set timer 0 and interrupt for 1 second
    /*TCCR0A = (1<<WGM01);
    OCR0A = 121;
    TIMSK0 = (1<<OCIE0A);
    TCCR0B = (1<<CS02) | (1<<CS00);*/

	//cli(); // disable interrupts
    sei(); // enable interrupts


	while(1)
	{
#ifdef TRANS	//transmitter
		mydelay(200);

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

		//build message
		u8_trans_buff[0]=0x80; // ID1
		u8_trans_buff[1]=0xFF; // ID2
		u8_trans_buff[2]=0x00; // ID3
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
		u8_trans_buff[19]=0x00; // crc1 place
		u8_trans_buff[20]=0x00; // crc2 place
		calculate_crc(21); // calculates the crc and puts it in the buffer

		//send
		send_cicad_message(21);



		/*test crc speed
		SET_M_EN;// it is on for 2us on 1MHz
		check_crc(0x01&count);
		if(u16_crc==0)CLEAR_M_EN;
		CLEAR_M_EN;
		count++;*/


#else	//receiver


		//cicad_receive();

		//test
		/*u8_trans_buff[0]='D'; // only one byte ID
		u8_trans_buff[1]='u'; // controll byte, no data
		u8_trans_buff[2]='s';
		u8_trans_buff[3]='a';
		u8_trans_buff[4]='n'; // crc1 place
		u8_trans_buff[5]=' ';*/



#endif
	}
	//Code should not reach here
	return 1;
}//End of code

////////////////////////////////////////////////////////////////////////////
