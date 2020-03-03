#!bin/sh

CC =gcc

ifeq ($(DEBUG),0)
	CFLAGS		+= -O2
else
	CFLAGS		+= -O0 -g
endif


BIN = dir_dup

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.C = %.o)

$(BIN):$(OBJS)
	   $(CC) $(CFLAGS) -o $@ $^ 


clean:
	rm -rf $(BIN)
	rm -rf *.o *.d	   

.PHONY: clean
