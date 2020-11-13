#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
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
  sem_t sem = (sem_t) malloc(sizeof(struct semaphore));

  sem->count = count;
  sem->queue_block = queue_create();
  sem->lock = 0;

  return sem;
}

int sem_destroy(sem_t sem)
{
  if (sem == NULL || queue_length(sem->queue_block) > 0) {
    return -1;
  }

  return 0;
}


int sem_down(sem_t sem)
{
  sem->lock = 0;
  return 0;

}

int sem_up(sem_t sem)
{
  sem->lock = 0;
  return 0;
}


