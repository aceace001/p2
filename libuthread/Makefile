# Target library
lib := libuthread.a 

all: $(lib)

objs    := queue.o uthread.o context.o sem.o preempt.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -MMD
CFLAGS  += -g

ifneq ($(V),1)
Q = @
endif

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

$(lib): $(objs)
	ar rcs $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN   $@"
	$(Q)rm -f $(lib) $(objs) $(deps)
