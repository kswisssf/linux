#!bin/sh

CC =gcc
LIBS = -lpthread

ifeq ($(DEBUG),0)
	CFLAGS		+= -O2
else
	CFLAGS		+= -O0 -g
endif


BIN = sample_pthread_sync

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.C = %.o)

$(BIN):$(OBJS)
	   $(CC) $(CFLAGS) -o $@ $^ $(LIBS)


clean:
	rm -rf $(BIN)
	rm -rf *.o *.d	   

.PHONY: clean
