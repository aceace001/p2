#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
  size_t count;
  queue_t queue_block;
};

sem_t sem_create(size_t count) {
  sem_t sem = (sem_t) malloc(sizeof(struct semaphore));

  sem->count = count;
  sem->queue_block = queue_create();

  return sem;
}

int sem_destroy(sem_t sem) {
  if (sem == NULL || queue_length(sem->queue_block) == -1) {
    return -1;
  }

  free(sem);

  return 0;
}

int sem_down(sem_t sem)
{
  struct uthread_tcb *thread = uthread_current();
  if (sem == NULL) {
    return -1;
  }
  while (sem->count < 1) {
    queue_enqueue(sem->queue_block, (void *)thread);
    uthread_block();
  }
  sem->count--;
  return 0;
}

int sem_up(sem_t sem)
{
  if (sem == NULL) {
    return -1;
  }

  sem->count++;
  if (queue_length(sem->queue_block) > 0) {
    struct uthread_tcb* thread = (struct uthread_tcb *) malloc(sizeof(struct uthread_tcb*));
    queue_dequeue(sem->queue_block, (void **)&thread);
    uthread_unblock(thread);

    free(thread);
  }
  return 0;
}
