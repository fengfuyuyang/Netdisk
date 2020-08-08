SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=server
CC:=gcc
$(ELF):$(OBJS)
	$(CC) -o $@ $^
.PHONY:rebuild clean
rebuild:clean $(OBJS) $(ELF)
clean:
	rm -rf $(OBJS) $(ELF)
