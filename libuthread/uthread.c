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

// thread control block
// contains a thread's context, stack pointer, and state
struct uthread_tcb {
	uthread_ctx_t *ctx;
	void *stack;
	enum states state;
};

struct uthread_tcb *curr_thread;
queue_t ready_queue;
queue_t block_queue;

struct uthread_tcb *uthread_current(void) {
	return curr_thread;
}

void uthread_yield(void) {
	struct uthread_tcb* prev_thread = (struct uthread_tcb *) malloc(sizeof(struct uthread_tcb));
	struct uthread_tcb* next_thread = (struct uthread_tcb *) malloc(sizeof(struct uthread_tcb));

	preempt_disable();
	prev_thread = curr_thread;

 	// put current thread into ready queue
	// context switch to the next thread in line
	queue_enqueue(ready_queue, curr_thread);
	queue_dequeue(ready_queue, (void**) &next_thread);

	curr_thread->state = READY;
	next_thread->state = RUN;

	curr_thread = next_thread;
	preempt_enable();
	uthread_ctx_switch(prev_thread->ctx, next_thread->ctx);
}

void uthread_exit(void) {
	// free current exiting thread's stack
	curr_thread->state = ZOMBIE;

	// if there are still threads in ready queue,
	// context switch to the next one in line
	if (queue_length(ready_queue) > 0) {
		struct uthread_tcb* next_thread = (struct uthread_tcb *) malloc(sizeof(struct uthread_tcb));

		preempt_disable();
		queue_dequeue(ready_queue, (void**) &next_thread);

		preempt_enable();
		uthread_ctx_switch(curr_thread->ctx, next_thread->ctx);
		uthread_ctx_destroy_stack(curr_thread->stack);
		curr_thread = next_thread;
	}
}

int uthread_create(uthread_func_t func, void *arg) {
	preempt_disable();
	struct uthread_tcb* new_thread = (struct uthread_tcb*) malloc (sizeof(struct uthread_tcb));
	new_thread->ctx = malloc(sizeof(uthread_ctx_t));
	new_thread->stack = uthread_ctx_alloc_stack();
	new_thread->state = READY;

	// newly created thread are enqueued in ready queue
	queue_enqueue(ready_queue, new_thread);

	preempt_enable();
	uthread_ctx_init(new_thread->ctx, new_thread->stack, func, arg);
	return 0;
}

int uthread_start(uthread_func_t func, void *arg) {
	// create the main idle thread
	struct uthread_tcb* idle_thread = (struct uthread_tcb*) malloc (sizeof(struct uthread_tcb));
	idle_thread->ctx = malloc(sizeof(uthread_ctx_t));
	
	if (idle_thread->ctx == NULL) {
		return -1;
	}
	idle_thread->stack = uthread_ctx_alloc_stack();
  
	if (idle_thread->stack == NULL) {
		return -1;
	}
       	idle_thread->state = RUN;
	curr_thread = idle_thread;
  
	// initialize the queues
	ready_queue = queue_create();
	block_queue = queue_create();

	preempt_start();
	preempt_enable();

	// create the first function thread
	uthread_create(func, arg);

  	// idle thread yields until there are no more threads in ready queue
	while (queue_length(ready_queue) > 0) {
		uthread_yield();
	}
	preempt_stop();

  	// destroy the queues
  	queue_destroy(ready_queue);
  	queue_destroy(block_queue);
  	return 0;
}

void uthread_block(void) {
	preempt_disable();
  	struct uthread_tcb* next_thread = (struct uthread_tcb *) malloc(sizeof(struct uthread_tcb));
   	
	// enqueue current thread into the block queue
  	queue_enqueue(block_queue, curr_thread);
  	curr_thread->state = BLOCKED;

  	// context switch to the next ready thread
  	queue_dequeue(ready_queue, (void**) &next_thread);
  	next_thread->state = RUN;
  	preempt_enable();
  	uthread_ctx_switch(curr_thread->ctx, next_thread->ctx);
  	curr_thread = next_thread;
}

void uthread_unblock(struct uthread_tcb *uthread) {
  	preempt_disable();
 	uthread->state = READY;
  	
	// remove uthread from block queue and put it into ready queue instead
  	queue_dequeue(block_queue, (void**) &uthread);
  	queue_enqueue(ready_queue, uthread);
  	preempt_enable();
}
