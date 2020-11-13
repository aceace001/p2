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

}

int sem_destroy(sem_t sem) {

}

int sem_down(sem_t sem)
{

}

int sem_up(sem_t sem)
{

}
