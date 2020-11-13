#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* queue destroy*/
void test_queue_destroy(void)
{
	int data = 3;
	queue_t q;

	fprintf(stderr, "*** TEST queue_destory ***\n");
	
	//test non empty queue, return -1 
	q = queue_create();
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_destroy(q) == -1);

	//test empty queue, return 0
	q = queue_create();
	TEST_ASSERT(queue_destroy(q) == 0);

	//test queue with NULL, return -1
	TEST_ASSERT(queue_destroy(NULL) == -1);
}

/*queue enqueue*/
void test_queue_enqueue(void)
{
	int data = 3;
	queue_t q;

	fprintf(stderr, "*** TEST queue_enqueue ***\n");

	//test enqueue with the address stored in data
	q = queue_create();
	TEST_ASSERT(queue_enqueue(q, &data) == 0);

	//test with data address that is NULL
	TEST_ASSERT(queue_enqueue(q, NULL) == -1);

	//test with q address that is NULL
	TEST_ASSERT(queue_enqueue(NULL, &data) == -1);
}

/*queue dequeue*/
void test_queue_dequeue(void)
{
	int *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_dequeue ***\n");

	//test dequeue with empty queue
	q = queue_create();
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);

	//test with data address that is NULL, by replacing itme with NULL
	TEST_ASSERT(queue_dequeue(q, NULL) == -1);

	//test dequeue q that is NULL
	TEST_ASSERT(queue_dequeue(NULL, (void**)&ptr) == -1);
}	

/*queue delete*/
void test_queue_delete(void)
{
	int data = 3;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete ***\n");
	
	//test delete with empty queue
	q = queue_create();
	TEST_ASSERT(queue_delete(q, &data) == -1);
	
	//test with data address that is NULL
	TEST_ASSERT(queue_delete(q, NULL) == -1);
}

/*function to increment items*/
static void inc_item(void *data)
{
	int *a = (int*)data;
	queue_t q;
	q = queue_create();
	
	if (*a == 5) {
		queue_delete(q, &data);
	} else {
		*a += 1;
	}
}

/*queue iterator test*/
void test_queue_iterator(void)
{
	int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int i;
	queue_t q;

	fprintf(stderr, "*** TEST queue_iterator ***\n");

	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
		queue_enqueue(q, &data[i]);
	}

	queue_iterate(q, inc_item);
	TEST_ASSERT(data[0] == 2);
}

/*queue length test*/
void test_queue_length(void)
{
	int data[] = {1, 2, 3, 4, 5};
	int i;
	queue_t q;

	fprintf(stderr, "*** TEST queue_length ***\n");
	
	//test length with empty queue, gets length of 0
	q = queue_create();
	TEST_ASSERT(queue_length(q) == 0);
	
	//test by enqueuing one queue item, gets length of 1
	queue_enqueue(q, &data[3]);
	TEST_ASSERT(queue_length(q) == 1);
	
	//gets length of 2
	queue_enqueue(q, &data[1]);
	TEST_ASSERT(queue_length(q) == 2);
	
	//destroy queue and get the length of the queue, which is 5
	queue_destroy(q);
	q = queue_create();
	
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
		queue_enqueue(q, &data[i]);
	}

	TEST_ASSERT(queue_length(q) == 5);
	
	//test with queue that is NULL
	TEST_ASSERT(queue_length(NULL) == -1);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_destroy();
	test_queue_enqueue();
	test_queue_dequeue();
	test_queue_delete();
	test_queue_iterator();
	test_queue_length();

	return 0;
}
