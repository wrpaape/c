CC		:= gcc
C_FLAGS		:= -I. -std=gnu99 -Wall -D__USE_FIXED_PROTOTYPES__
RM		:= rm
RM_FLAGS	:= -rf


all: brackets

brackets: brackets.c
	$(CC) $(C_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) brackets
