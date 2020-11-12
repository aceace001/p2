# Target library
lib 	:= libuthread.a
objs    := preempt.o context.o uthread.o queue.o sem.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -MMD
CFLAGS  += -02

ifneq ($(V),1)
Q = @
endif

all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include$(deps)

$(lib): $(objs)
	$(Q)$(CC) $(CFLAGS) -o $@ $^
  
%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN  $@"
	$(Q)rm -f $(objs) $(deps) *.a
