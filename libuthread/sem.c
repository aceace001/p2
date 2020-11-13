#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
	size_t count;
	queue_t queue_block;
	int lock;
};

int test_and_set (int *mem) {
	int oldval = *mem;
	*mem = 1;

	return oldval;
}

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem = (sem_t) malloc(sizeof(struct semaphore));

	sem->count = count;
	sem->queue_block = queue_create();
	sem->lock = 0;

	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL || queue_length(sem->queue_block) > 0) {
		return -1;
	}

	free(sem);

	return 0;
}


int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	pthread_t tid = pthread_self();

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

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	struct uthread_tcb *tid;

	if (sem == NULL) {
		return -1;
	}
	
	while (test_and_set(&sem->lock) == 1) {
		sem->count += 1;
		if (queue_length(sem->queue_block) > 0) {
			queue_dequeue(sem->queue_block, (void*) &tid);
			uthread_unblock(tid);
		}
	}

	sem->lock = 0;

	return 0;
}



