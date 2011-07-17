#ifndef _port_H__
#define _port_H__

#define SET_LED_R		set_bit(PORTB, PB1)
#define CLEAR_LED_R		clear_bit(PORTB, PB1)
#define SET_LED_G		set_bit(PORTB, PB0)
#define CLEAR_LED_G		clear_bit(PORTB, PB0)

#define SET_OPTO_EN		set_bit(PORTA, PA6)
#define CLEAR_OPTO_EN	clear_bit(PORTA, PA6)

#define OPTO_1_STATE	test_bit(PINA,PINA7)
#define OPTO_2_STATE	test_bit(PINB,PINB2)

#define IR_REC_RELEASED	test_bit(PINA,PINA0)

#define SET_IR_REC_EN	set_bit(PORTA, PA1)
#define CLEAR_IR_REC_EN	clear_bit(PORTA, PA1)

#define SET_M_BRAKE		set_bit(PORTA, PA3)
#define CLEAR_M_BRAKE	clear_bit(PORTA, PA3)

#define SET_IR_EM		set_bit(PORTA, PA4)
#define CLEAR_IR_EM		clear_bit(PORTA, PA4)

#define SET_M_EN		set_bit(PORTA, PA5)
#define CLEAR_M_EN		clear_bit(PORTA, PA5)

#define SET_TX SET_LED_R
#define CLEAR_TX CLEAR_LED_R

#endif

