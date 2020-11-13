# REPORT
---
## Design

### Phase 1: queue
### Phase 2: uthread
Threads are represented by TCB (Thread Control Block), and each TCB contains the thread's context, its stack, and its current state. We declare global variables `curr_thread` that points to the current running thread, 'block_queue' to store blocked threads and a 'ready_queue' to store the ready threads. When the thread library is initiated by calling `uthread_start()`, the idle thread is first created as our running thread, which represent the main thread for our library. This is the only thread where we do not need to initialize its context. The 2 queues are created before `uthread_create()` is then called to create our first ready thread based on the functions and arguments. The idle thread then enters an loop which yields to our freshly created ready thread immediately, thus kicking things off for our multithreaded library. A thread yields when `uthread_yield()` is called which put the current running thread into the `ready_queue` and then grabs the first ready thread in the queue and run it. The idle thread will run the loop and yield indefinitely until there are no more threads in the `ready_queue`, in which case it can safely destroy the queues and return. A thread may also enter the `block_queue` when it is blocked by a semaphore, in which case the current running thread context switch to the next available thread and the blocked thread can no longer be ready until it is unlocked. When a thread is finished and `uthread_exit()` is called, it frees the stack and also check if the `ready_queue` is empty and if it isn't it will context switch to the first ready thread and run.

### Phase 3: semaphore

### Phase 4: preemption
j

## Shortcomings
---
## Conclusion
