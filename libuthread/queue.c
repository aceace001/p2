#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node {
	void* data;
	struct node* next;
	struct node* previous;
};

struct queue {
	/* TODO Phase 1 */
	int length;
	struct node* head;
	struct node* tail;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t queue = malloc(sizeof(struct queue));
	
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
	/* TODO Phase 1 */
	if (queue == NULL || queue->length > 0) {
		return -1;
	} else {
		free(queue);
		return 0;
	}
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	struct node* enqueue_node = (struct node*)malloc(sizeof(struct node));

	if (queue == NULL || data == NULL) {
		return -1;
	}

	if (enqueue_node == NULL) {
		return -1;
	}

	enqueue_node->data = data;
	enqueue_node->next = NULL;
	enqueue_node->previous = NULL;

	if (queue->length == 0) {
		queue->head = enqueue_node;
		queue->tail = queue->head;
		queue->length = queue->length + 1;
	} else {
		queue->tail->next = enqueue_node;
		queue->tail = queue->tail->next;
		queue->length = queue->length + 1;
	}
	
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	struct node* dequeue_node = malloc(sizeof(struct node));

	if (queue == NULL || data == NULL || queue->length == 0) {
		return -1;
	}

	if (dequeue_node == NULL) {
		return -1;
	}

	*data = queue->head->data;
	dequeue_node = queue->head->next;
	free(queue->head);
	queue->head = dequeue_node;
	queue->length = queue->length - 1;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	struct node* current_node = malloc(sizeof(struct node));
	current_node = queue->head;

	if (queue == NULL || data == NULL || queue->length == 0) {
		return -1;
	}

	while(current_node != NULL) {
		if (current_node->data == data) {
			current_node->previous->next = current_node->next;
			current_node->next->previous = current_node->previous;
			free(current_node);
			queue->length = queue->length - 1;

			return 0;
		}
		current_node = current_node->next;
	}

	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if (queue == NULL || func == NULL) {
		return -1;
	}
	
	struct node* iterate_node = queue->head;

	while (iterate_node != NULL) {
		if (func(iterate_node->data, arg) == 1) {
			*data = iterate_node->data;
			return 0;
		} else {
			iterate_node = iterate_node->next;
		}
	}

	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue == NULL) {
		return -1;
	}
	return queue->length;
}

