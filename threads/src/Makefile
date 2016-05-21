.PHONY: all clean

# relative directories
# ==============================================================================
ROOT_DIR = ..
SRC_DIR  = $(ROOT_DIR)/src
OBJ_DIR  = $(ROOT_DIR)/obj
BIN_DIR  = $(ROOT_DIR)/bin
TEST_DIR = $(ROOT_DIR)/test
TRNR_DIR = $(TEST_DIR)/test_runners


# user config
# ==============================================================================
CC          = gcc
CFLAGS      = -g -I$(SRC_DIR) -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
AR          = ar
ARFLAGS     = rcs
SRC_LIBS    = queue string
TEST_LIBS   = unity
SRC_LFLAGS  = $(addprefix -l, $(SRC_LIBS))
TEST_LFLAGS = $(addprefix -l, $(TEST_LIBS))
TRNR_SCRIPT = $(HOME)/my_projects/c/unity/auto/generate_test_runner.rb

# user targets
# ==============================================================================
# root/bin/threads
THREADS      = threads
THREADS_DIR  = $(SRC_DIR)
THREADS_SRC  = $(addprefix $(THREADS_DIR)/, $(addsuffix .c, $(THREADS)))
THREADS_HDR  = $(addprefix $(THREADS_DIR)/, $(addsuffix .h, $(THREADS)))
THREADS_OBJ  = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(THREADS)))
THREADS_BIN  = $(addprefix $(BIN_DIR)/, $(THREADS))
THREADS_ODEP = $(THREADS_SRC) $(THREADS_HDR)
THREADS_BDEP = $(THREADS_OBJ)

# root/test/threads_test
THREADS_TEST      = $(addsuffix _test, $(THREADS))
THREADS_TRNR      = $(addsuffix _runner, $(THREADS_TEST))
THREADS_TEST_DIR  = $(TEST_DIR)
THREADS_TEST_SRC  = $(addprefix $(THREADS_TEST_DIR)/, $(addsuffix .c, $(THREADS_TEST)))
THREADS_TRNR_SRC  = $(addprefix $(TRNR_DIR)/, $(addsuffix .c, $(THREADS_TRNR)))
THREADS_TEST_OBJ  = $(addprefix $(OBJ_DIR)/,  $(addsuffix .o, $(THREADS_TEST)))
THREADS_TRNR_OBJ  = $(addprefix $(OBJ_DIR)/,  $(addsuffix .o, $(THREADS_TRNR)))
THREADS_TEST_BIN  = $(addprefix $(TEST_DIR)/, $(THREADS_TEST))
THREADS_TEST_ODEP = $(THREADS_TEST_SRC) $(THREADS_ODEP)
THREADS_TRNR_ODEP = $(THREADS_TRNR_SRC)
THREADS_TEST_BDEP = $(THREADS_TEST_OBJ) $(THREADS_TRNR_OBJ)


# target groups
# ==============================================================================
EXPAND_GROUP  = $(foreach item, $(patsubst %, %_$1, $2), $($(item)))

ITEMS       = THREADS
SRC_OBJS    = $(call EXPAND_GROUP,OBJ,$(ITEMS))
SRC_BINS    = $(call EXPAND_GROUP,BIN,$(ITEMS))
TRNR_SRCS   = $(call EXPAND_GROUP,TRNR_SRC,$(ITEMS))
TRNR_OBJS   = $(call EXPAND_GROUP,TRNR_OBJ,$(ITEMS))
TEST_OBJS   = $(call EXPAND_GROUP,TEST_OBJ,$(ITEMS))
TEST_BINS   = $(call EXPAND_GROUP,TEST_BIN,$(ITEMS))
ALL_TARGETS = $(SRC_OBJS) $(SRC_BINS) $(TRNR_SRCS) $(TRNR_OBJS) $(TEST_OBJS) $(TEST_BINS)


# make targets
# ==============================================================================
all: $(ALL_TARGETS)


# root/obj/threads.o
$(THREADS_OBJ): $(THREADS_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

# root/bin/threads
$(THREADS_BIN): $(THREADS_BDEP)
	$(CC) $(CFLAGS) $(SRC_LFLAGS) -o $@ $^


# root/test/test_runners/threads_test_runner.c
$(THREADS_TRNR_SRC):
	ruby $(TRNR_SCRIPT) $(THREADS_TEST_SRC) $(THREADS_TRNR_SRC)

# root/obj/threads_test_runner.o
$(THREADS_TRNR_OBJ): $(THREADS_TRNR_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

# root/obj/threads_test.o
$(THREADS_TEST_OBJ): $(THREADS_TEST_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

# root/test/threads_test
$(THREADS_TEST_BIN): $(THREADS_TEST_BDEP)
	$(CC) $(CFLAGS) $(SRC_LFLAGS) $(TEST_LFLAGS) -o $@ $^


clean:
	$(RM) $(ALL_TARGETS)
