SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	$(CC) -o $@ $^ -lcrypt
.PHONY:rebuild clean
rebuild:clean $(OBJS) $(ELF)
clean:
	rm -rf $(OBJS) $(ELF)
