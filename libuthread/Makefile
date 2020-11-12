# Target library
lib := libuthread.a 

all: $(lib)
	ar rcs libuthread.a queue.o uthread.o context.o sem.o preempt.o

targets := libuthread.a
objs    := queue.o uthread.o context.o sem.o preempt.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g
PANDOC  := pandoc

ifneq ($(V),1)
Q = @
endif

libuthread.a: $(objs)
	ar rcs libuthread.a  $@ $^

%.o: %.c %.h
	@echo "CC   $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN   $@"
	$(Q)rm -f $(targets) $(objs)