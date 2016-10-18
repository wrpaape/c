CC		:= gcc
C_FLAGS		:= -I. -std=gnu99 -Wall -D__USE_FIXED_PROTOTYPES__
RM		:= rm
RM_FLAGS	:= -rf


all: red_black two_three_four

red_black: red_black.c
	$(CC) $(C_FLAGS) $< -o $@

two_three_four: two_three_four.c two_three_four.h
	$(CC) $(C_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) red_black two_three_four
