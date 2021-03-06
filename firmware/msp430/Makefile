# Makefile created by gnomad

PROG = main
CC = msp430-gcc
CXX = msp430-g++
OBJDUMP = msp430-objdump
SIZE = msp430-size
MSPDEBUG = mspdebug
CFLAGS = -O0 -Wall -mmcu=msp430g2553
FET = rf2500
GDB = msp430-gdb
GDBTUI = $(GDB)tui

# Uncomment the following to enable debugging
#CFLAGS += -g -DDEBUG

OBJS=$(PROG).o cicad_physical.o cicad.o

all: $(PROG).elf  $(PROG).lst
	$(SIZE) $(PROG).elf

.PHONY: all

$(PROG).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG).elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.lst: %.elf
	$(OBJDUMP) -DS $< >$@

clean:
	rm -fr $(PROG).elf $(PROG).lst $(OBJS)

install: $(PROG).elf
	$(MSPDEBUG) $(FET) "prog $(PROG).elf"

mspdebug: $(PROG).elf
	$(MSPDEBUG) $(FET)

debug: $(PROG).elf
	$(MSPDEBUG) $(FET) gdb

gdb: $(PROG).elf
	$(GDB) $(PROG).elf
 
tui: $(PROG).elf
	$(GDBTUI) $(PROG).elf
 
ddd: $(PROG).elf
	ddd --debugger $(GDB) $(PROG).elf