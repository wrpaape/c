#include <unity/unity.h>
#include "threads.h"

static struct Queue *queue;

void setUp(void)
{
	queue = init_queue();
}

void tearDown(void)
{
	free(queue);
}

void test_queue_interface(void)
{
	TEST_ASSERT_EQUAL(NULL, queue_pop(queue));

	queue_push(queue, (void *) 42);
	queue_push(queue, "ooga booga");
	queue_push(queue, &queue_push);

	TEST_ASSERT_EQUAL_INT(42,	       queue_pop(queue));
	TEST_ASSERT_EQUAL_STRING("ooga booga", queue_pop(queue));

	queue_push(queue, (void *) 13);
	queue_push(queue, "bar");

	TEST_ASSERT_EQUAL((void *) &queue_push, queue_pop(queue));
	TEST_ASSERT_EQUAL_INT(13,		queue_pop(queue));
	TEST_ASSERT_EQUAL_STRING("bar",		queue_pop(queue));
}
