/*
 * Slight modified yield test
 *
 * This is nearly identical to the yield test except that we do not yield before
 * printing thread 3. The program should output:
 *
 * thread1
 * thread3
 * thread2
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

void thread3(void *arg)
{
  printf("thread3\n");
}

void thread2(void *arg)
{
  uthread_create(thread3, NULL);
  uthread_yield();
  printf("thread2\n");
}

void thread1(void *arg)
{
  uthread_create(thread2, NULL);
  uthread_yield();
  printf("thread1\n");
  uthread_yield();
}

int main(void)
{
  uthread_start(thread1, NULL);
  return 0;
}
