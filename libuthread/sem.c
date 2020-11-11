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
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem = (sem_t) malloc(sizeof(struct semaphore));

	sem->count = count;
	sem->queue_block = queue_create();
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

	while (sem->count == 0) {
		queue_enqueue(sem->queue_block, &tid);
		thread_block();
	}

	sem->count -=1;

	return 0;

}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	pthread_t tid;

	if (sem == NULL) {
		return -1;
	}

	sem->count += 1;
	if (queue_length(sem->queue_block) > 0) {
		queue_dequeue(sem->queue_block, (void*) &tid);
		thread_unblock(tid);
	}

	return 0;
}

