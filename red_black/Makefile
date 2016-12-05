CC		:= gcc
CC_FLAGS	:= -std=gnu99 -I. -Wall -c
LD		:= ld
LD_FLAGS	:= -arch x86_64 -macosx_version_min 10.11 -no_pie
RM		:= rm
RM_FLAGS	:= -rf


all: red_black

red_black: key_utils.o red_black_utils.o red_black.o
	$(LD) $(LD_FLAGS) $^ -lc -o $@

red_black.o: red_black.c red_black.h red_black_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

red_black_utils.o: red_black_utils.c red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

key_utils.o: key_utils.c key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) red_black red_black.o red_black_utils.o key_utils.o
