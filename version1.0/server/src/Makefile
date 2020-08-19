SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=ftpserver
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	$(CC) -o $@ $^ -pthread -lcrypt
.PHONY:rebuild clean
rebuild:clean $(OBJS) $(ELF)
clean:
	rm -rf $(OBJS) $(ELF)
