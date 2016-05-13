.PHONY: all clean

# relative directories
# ==============================================================================
ROOT_DIR = ..
SRC_DIR  = $(ROOT_DIR)/src
OBJ_DIR  = $(ROOT_DIR)/obj
BIN_DIR  = $(ROOT_DIR)/bin
TEST_DIR = $(ROOT_DIR)/test


# user config
# ==============================================================================
CC          = gcc
CFLAGS      = -g -I$(SRC_DIR) -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
AR          = ar
ARFLAGS     = rcs
SRC_LIBS    =
TEST_LIBS   = unity
SRC_LFLAGS  = $(addprefix -l, $(SRC_LIBS))
TEST_LFLAGS = $(addprefix -l, $(TEST_LIBS))


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
THREADS_TEST      = $(add_suffix _test, $(THREADS))
THREADS_TEST_DIR  = $(TEST_DIR)
THREADS_TEST_SRC  = $(addprefix $(THREADS_TEST_DIR)/, $(addsuffix .c, $(THREADS_TEST)))
THREADS_TEST_OBJ  = $(addprefix $(OBJ_DIR)/,  $(addsuffix .o, $(THREADS_TEST)))
THREADS_TEST_BIN  = $(addprefix $(TEST_DIR)/, $(THREADS_TEST))
THREADS_TEST_ODEP = $(THREADS_TEST_SRC)
THREADS_TEST_BDEP = $(THREADS_TEST_OBJ)


# target groups
# ==============================================================================
EXPAND_GROUP = $(foreach item, $(patsubst %, %_$1, $2), $($(item)))

ITEMS       = THREADS
SRC_OBJS    = $(call EXPAND_GROUP,OBJ,$(ITEMS))
SRC_BINS    = $(call EXPAND_GROUP,BIN,$(ITEMS))
TEST_OBJS   = $(call EXPAND_GROUP,TEST_OBJ,$(ITEMS))
TEST_BINS   = $(call EXPAND_GROUP,TEST_BIN,$(ITEMS))
ALL_TARGETS = $(SRC_OBJS) $(TEST_OBJS) $(SRC_BINS) $(TEST_BINS)


# make targets
# ==============================================================================
all: $(ALL_TARGETS)


# root/bin/threads
$(THREADS_BIN): $(THREADS_BDEP)
	$(CC) $(CFLAGS) $(SRC_LFLAGS) -o $@ $^

# root/obj/threads.o
$(THREADS_OBJ): $(THREADS_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<


# root/test/threads_test
$(THREADS_TEST_BIN): $(THREADS_TEST_BDEP)
	$(CC) $(CFLAGS) $(SRC_LFLAGS) $(TEST_LFLAGS) -o $@ $^

# root/obj/threads_test.o
$(THREADS_TEST_OBJ): $(THREADS_TEST_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	$(RM) $(ALL_TARGETS)
