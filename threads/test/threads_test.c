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
	TEST_ASSERT_EQUAL(NULL, pop_data(queue));

	push_data(queue, (void *) 42);
	push_data(queue, "ooga booga");
	push_data(queue, &push_data);

	TEST_ASSERT_EQUAL_INT(42,	       pop_data(queue));
	TEST_ASSERT_EQUAL_STRING("ooga booga", pop_data(queue));

	push_data(queue, (void *) 13);
	push_data(queue, "bar");

	TEST_ASSERT_EQUAL((void *) &push_data, pop_data(queue));
	TEST_ASSERT_EQUAL_INT(13,	       pop_data(queue));
	TEST_ASSERT_EQUAL_STRING("bar",	       pop_data(queue));
}
