# **ECS150 Project2 Report**

## Phase 1: queue

### struct node (items of queue)
Create the structure to represent each node of the queue, and each node holds 
the data and pointer that points to the next and previous node in the queue.

### struct queue
Create the structure to represent the queue, and the pointers that points to the
first(head) and last(tail) items in the queue. It also stores the length of the
queue.

### create queue
Initialize empty queue with head and tail points to NULL, and length of 0 for 
the queue.

### enqueue
Create a new node in order to link it to the end of the queue (tail) by 
allocatingspace and initialize it. If the length of the queue is 0, then the 
head and tail of the queue are both the new node created.Also add 1 to the 
length after enqueue.

### dequeue
Remove the node by storing the data to another pointer, free the head of the 
node, then subtract 1 to the length after dequeue.

### delete
We iterate the queue until the data specified is found, then unlink it from the
queue and delete it. However, in the testing, we found out that it sometimes
will get segmentation fault after we run `queue_delete()`, so we think that 
our implementation is incomplete for delete.

### iteration
using a while loop to iterate through the queue, and then call the callback 
function `func()` on the data item to increment the item. The implementation for
increment is in the tester file.

### testing
We added testing for all the queue functions in `queue_tester.c`. We mostly 
tested whether there are any errors related to the nonexistence of queue and 
data. There might be some errors in `queue_delete()` function since we sometimes
get "core dumped" when performing delete operations. And due to the incomplete 
of delete, there might also be errors when testing `queue_iterate()` function.

## Phase 2: uthread
* Threads are represented by TCB (Thread Control Block), and each TCB contains 
the thread's context, its stack, and its current state. We declare global 
variables `curr_thread` that points to the current running thread, 'block_queue'
to store blocked threads and a 'ready_queue' to store the ready threads. 

* When the thread library is initiated by calling `uthread_start()`, the idle 
thread is first created as our running thread, which represent the main thread 
for our library. This is the only thread where we do not need to initialize its 
context. The 2 queues are created before `uthread_create()` is then called to 
create our first ready thread based on the functions and arguments. The idle 
thread then enters an loop which yields to our freshly created ready thread 
immediately, thus kicking things off for our multithreaded library. 

* A thread yields when `uthread_yield()` is called which put the current running
thread into the `ready_queue` and then grabs the first ready thread in the queue
and run it. The idle thread will run the loop and yield indefinitely until there
are no more threads in the `ready_queue`, in which case it can safely destroy 
the queues and return. 

* A thread may also enter the `block_queue` when it is blocked by a semaphore, 
in which case the current running thread context switch to the next available 
thread and the blocked thread can no longer be ready until it is unlocked. 

* When a thread is finished and `uthread_exit()` is called, it frees the stack 
and also check if the `ready_queue` is empty and if it isn't it will context 
switch to the first ready thread and run.

## Phase 3: semaphore

### create semaphore
create semaphore by allocating the size for sem structure, then initialize count
of available locks. We also initialize the lock to 0 in order to use it in 
test and set functions in `sem_up` and `sem_down` functions

### sem down 
We decided to implement sem down function using spinlock by creating a 
`test_and_set` function. If the count of lock is 0, meaning there is no 
resources available, thread will be blocked until the count is not 0. This might
be a wrong implementation since the queue will not be running `uthread_block()`
fucntion inside while loop, meaning there was never a block, and we failed half 
of the test cases. 

## Phase 4: preemption
* Preemption is implemented through `sigaction` and `itimerval`. When 
`preempt_start()` gets called, it will initiate an alarm that forces the current
running thread to yield every 10000 microseconds, which is 100 times a second. 
We also needs to store the previous signals and timers so when `preempt_stop()` 
gets called they will get restored. 

* However, we do not want preemption to interrupt us when we are in a critical 
section, so we must also implement `preempt_enable()` and `preempt_disable()` 
to make sure the timer does not interrupt our atomic flow of action when we 
needed it. We always call `preempt_disable()` before operations such as 
enqueueing and dequeueing so order of operations are not messed up.

## Shortcomings and Conclusion
* Our uthread implementations could be a bit more robust when handling errors 
and such. It could also use some work at cleaning the malloc data and freeing 
the pointers. 

* Our semaphore implementation also still needs a bit more work; the `sem_up()` 
and `sem_down()` functions does not correctly block and unblock the correct 
threads sometimes. 

* For preemption, while we are happy with our implementation, our tester seems 
a bit silly and might not actually accurately demonstrate the preemptiveness of
our library. 

* Overall, we are not 100% satisfied with our final product. Compared to last 
project, this project definitely required a lot more thinking and tinkering to 
get right, as there are quite a bit of intricate details we must take care of. 
This was a good test of not only our coding skills but also our understanding of
multithreading and synchronizations.
