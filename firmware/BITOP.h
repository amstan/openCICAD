#define clear_bit( reg, bitNumb )	((reg) &= ~(1 << (bitNumb)))
#define set_bit( reg, bitNumb )		((reg) |= (1 << (bitNumb)))
#define test_bit( reg, bitNumb )	((reg) & (1 << (bitNumb))) //put 1 in specific position
#define change_bit(ptr, position, val)	{if (val)  set_bit(ptr, position); else	clear_bit(ptr,position);}


//void change_bit (char *ptr, unsigned char position,unsigned char val);
