#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*struct for the items of the queue, 
  have data and pointers to the previous or next item
*/
struct node {
	void* data;
	struct node* next;
	struct node* previous;
};

/*struct for the queue, have pointers to the first and last 
  items of the queue, and also have length of the queue
*/
struct queue {
	int length;
	struct node* head;
	struct node* tail;
};

queue_t queue_create(void)
{
	//allocate space for queue
	queue_t queue = malloc(sizeof(struct queue));
	
	//initialize empty queue, pointer to length init to 0, head and tail init to NULL
	if (queue != NULL) {
		queue->length = 0;
		queue->head = NULL;
		queue->tail = NULL;
		
		return queue;
	} else {
		return NULL;
	}
}

int queue_destroy(queue_t queue)
{
	//if queue is NULL or there are still items in the queue, then cannot destroy
	if (queue == NULL || queue->length > 0) {
		return -1;
	} else {
		//deallocate space for queue
		free(queue); 
		return 0;
	}
}

int queue_enqueue(queue_t queue, void *data)
{
	//new node in order to enqueue it to queue
	struct node* enqueue_node = (struct node*)malloc(sizeof(struct node));

	//if queue or data is NULL, then cannot enqueue
	if (queue == NULL || data == NULL) {
		return -1;
	}
	
	//cannot enqueue if new node is NULL
	if (enqueue_node == NULL) {
		return -1;
	}

	//init new node to NULL
	enqueue_node->data = data;
	enqueue_node->next = NULL;
	enqueue_node->previous = NULL;
	
	//if length of queue is 0, the head and tail of queue are the new node
	if (queue->length == 0) {
		queue->head = enqueue_node;
		queue->tail = queue->head;
		queue->length = queue->length + 1;
	} else {
		//if not, enqueue the new node to the end of queue 
		queue->tail->next = enqueue_node;
		queue->tail = queue->tail->next;
		queue->length = queue->length + 1;
	}
	
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	struct node* dequeue_node = malloc(sizeof(struct node));
	
	//if queue or data is NULL, or queue is empty, then cannot dequeue
	if (queue == NULL || data == NULL || queue->length == 0) {

		return -1;
	}

	if (dequeue_node == NULL) {
		return -1;
	}

	//data is pointed to the head of queue
	*data = queue->head->data;
	dequeue_node = queue->head->next;
	free(queue->head);
	queue->head = dequeue_node;
	queue->length = queue->length - 1;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	
	struct node* current_node = malloc(sizeof(struct node));
	current_node = queue->head;

	//if queue or data is NULL, nor length of queue is empty, then cannot delete
	if (queue == NULL || data == NULL || queue->length == 0) {
		return -1;
	}

	//iterate through the queue and find the data required
	while(current_node != NULL) {
		if (current_node->data == data) {
			//if data found, then unlink it from queue and delete it.
			current_node->previous->next = current_node->next;
			current_node->next->previous = current_node->previous;
			queue->length = queue->length - 1;
			free(current_node);

			return 0;
		}
		//increament current node
		current_node = current_node->next;
	}

	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	//cannot iterate if queue or data is NULL
	if (queue == NULL || func == NULL) {
		return -1;
	}
	
	struct node* iterate_node = queue->head;

	//iterate through the queue, and call func on the data item
	while (iterate_node != NULL) {
		func(iterate_node->data);
		return 0;
	}

	return 0;
}

int queue_length(queue_t queue)
{
	//cannot get length if queue is NULL
	if (queue == NULL) {
		return -1;
	}
	return queue->length;
}

