# Target library
lib := libuthread.a 

all: $(lib)

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

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN   $@"
	$(Q)rm -f $(targets) $(objs)
