CC		:= gcc
C_FLAGS		:= -I. -std=gnu99 -Wall -D__USE_FIXED_PROTOTYPES__
RM		:= rm
RM_FLAGS	:= -rf


all: two_three_four_test

# red_black: red_black.c
# 	$(CC) $(C_FLAGS) $< -o $@

two_three_four.o: two_three_four.c two_three_four.h
	$(CC) $(C_FLAGS) -DPAGE_SIZE=4096 -c $< -o $@

two_three_four_test.o: two_three_four_test.c two_three_four_test.h two_three_four.h
	$(CC) $(C_FLAGS) -c $< -o $@

two_three_four_test: two_three_four_test.o two_three_four.o
	$(CC) $(C_FLAGS) $^ -o $@

clean:
	$(RM) $(RM_FLAGS) two_three_four.o two_three_four_test.o two_three_four_test
