#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

//struct to keep track of resources, and have a lock to implement test and set function
struct semaphore {
	size_t count;
	queue_t queue_block;
	int lock;
};

//test and set function to implement lock to use in sem_up and sem_down 
int test_and_set (int *mem) {
	int oldval = *mem;
	*mem = 1;

	return oldval;
}

//initialize queue for semaphore
sem_t sem_create(size_t count)
{
	//allocate spapce for sem
	sem_t sem = (sem_t) malloc(sizeof(struct semaphore));

	sem->count = count;
	sem->queue_block = queue_create();
	sem->lock = 0;

	return sem;
}

int sem_destroy(sem_t sem)
{
	//if sem is NULL or the queue of sem is not empty, then cannot destroy
	if (sem == NULL || queue_length(sem->queue_block) > 0) {
		return -1;
	}

	free(sem);

	return 0;
}

//uses test and set function to implement spinlock 
//if count is 0, meaning there is no resources available, thread is block
//until count is not 0.
int sem_down(sem_t sem)
{
	struct uthread_tcb *tid = uthread_current();

	//cannot sem down if sem is NULL
	if (sem == NULL) {
		return -1;
	}

	while (test_and_set(&sem->lock) == 1){
		while (sem->count == 0) {
		queue_enqueue(sem->queue_block, &tid);
		uthread_block();
		}

	sem->count -=1;
	}

	sem->lock = 0;

	return 0;

}

//also uses test and set function to implement spinlock
int sem_up(sem_t sem)
{
	struct uthread_tcb *tid;

	//cannot sem up if sem is NULL
	if (sem == NULL) {
		return -1;
	}
	
	while (test_and_set(&sem->lock) == 1) {
		sem->count += 1;
		if (queue_length(sem->queue_block) > 0) {
			queue_dequeue(sem->queue_block, (void*)&tid);
			uthread_unblock(tid);
		}
	}

	sem->lock = 0;

	return 0;
}
