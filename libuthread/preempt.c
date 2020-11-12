#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction act;
struct itimerval timer;
struct sigaction act_old;
struct itimerval timer_old;

void handler() {
  uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
  sigprocmask(SIG_BLOCK, &act.sa_mask, &act_old.sa_mask);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
  sigprocmask(SIG_UNBLOCK, &act.sa_mask, &act_old.sa_mask);
}

void preempt_start(void)
{
	/* TODO Phase 4 */
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGVTALRM);

  act.sa_handler = &handler;
  sigaction(SIGVTALRM, &act, &act_old);

  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 1000000/HZ;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 1000000/HZ;
  setitimer(ITIMER_VIRTUAL, &timer, &timer_old);
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESETHAND;
  sigaction(SIGVTALRM, &act_old, NULL);
  setitimer(ITIMER_REAL, &timer_old, NULL);
}

