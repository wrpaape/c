#include <stdio.h>
#include <setjmp.h>


int global_int;

jmp_buf jump_buffer;

void
do_it(void)
{
	static int do_it_times = 10;

	int stack_buffer[1024];

	stack_buffer[0]    = 1;
	stack_buffer[1023] = 1;

	--do_it_times;

	if (do_it_times >= 0)
		do_it();
	else
		longjmp(jump_buffer,
			1);

	printf("should not see: %d, %d\n",
	       stack_buffer[0],
	       stack_buffer[1023]);
}

#define SET_INTS(VALUE)							\
global_int = static_int = volatile_int = register_int = auto_int = VALUE

#define PRINT_INTS()							\
printf("global_int: %d, static_int: %d, volatile_int: %d, "		\
       "register_int: %d, auto_int: %d\n",				\
       global_int, static_int, volatile_int, register_int, auto_int)

int
main(void)
{
	static int static_int;
	volatile int volatile_int;
	register int register_int;
	auto int auto_int;

	SET_INTS(0);

	PRINT_INTS();

	puts("doin it");

	if (setjmp(jump_buffer) == 0) {
		SET_INTS(1);
		PRINT_INTS();
		do_it();

	} else {
		puts("did it");
		PRINT_INTS();
	}


	return 0;
}
