#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

enum states { RUN, READY, BLOCKED, ZOMBIE };

struct uthread_tcb {
  uthread_ctx_t *ctx;
  void *stack;
  enum states state;
};

struct uthread_tcb *curr_thread;
queue_t ready_queue;

struct uthread_tcb *uthread_current(void) {
  return curr_thread;
}

void uthread_yield(void) {
  /* TODO Phase 2 */
  struct uthread_tcb* next_thread = (struct uthread_tcb *) malloc(sizeof(struct uthread_tcb));

  queue_enqueue(ready_queue, curr_thread);
  queue_dequeue(ready_queue, (void**) &next_thread);
  curr_thread->state = READY;
  next_thread->state = RUN;
  uthread_ctx_switch(curr_thread->ctx, next_thread->ctx);
  curr_thread = next_thread;
}

void uthread_exit(void) {
  /* TODO Phase 2 */
  queue_dequeue(ready_queue, (void **)&curr_thread);
}

int uthread_create(uthread_func_t func, void *arg) {
  /* TODO Phase 2 */
  struct uthread_tcb* new_thread = (struct uthread_tcb*) malloc (sizeof(struct uthread_tcb));
  new_thread->ctx = malloc(sizeof(uthread_ctx_t));
  new_thread->stack = uthread_ctx_alloc_stack();
  new_thread->state = READY;
  uthread_ctx_init(new_thread->ctx, new_thread->stack, func, arg);
  queue_enqueue(ready_queue, new_thread);
  return 0;
}

int uthread_start(uthread_func_t func, void *arg) {
  /* TODO Phase 2 */
  struct uthread_tcb* idle_thread = (struct uthread_tcb*) malloc (sizeof(struct uthread_tcb));
  idle_thread->ctx = malloc(sizeof(uthread_ctx_t));
  idle_thread->stack = uthread_ctx_alloc_stack();
  idle_thread->state = RUN;
  curr_thread = idle_thread;
  ready_queue = queue_create();
  uthread_create(func, arg);
  while (queue_length(ready_queue) > 0) {
    uthread_yield();
  }
  return 0;
}

void uthread_block(void) {
  /* TODO Phase 2/3 */

}

void uthread_unblock(struct uthread_tcb *uthread) {
  /* TODO Phase 2/3 */
}

