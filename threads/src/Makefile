.PHONY: all clean

# relative directories
# ==============================================================================
ROOT_DIR = ..
SRC_DIR  = $(ROOT_DIR)/src
OBJ_DIR  = $(ROOT_DIR)/obj
BIN_DIR  = $(ROOT_DIR)/bin
TEST_DIR = $(ROOT_DIR)/tests


# user config
# ==============================================================================
CC      = gcc
CFLAGS  = -g -I$(SRC_DIR) -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
AR      = ar
ARFLAGS = rcs


# user targets
# ==============================================================================
# root/bin/threads
THREADS      = threads
THREADS_DIR  = $(SRC_DIR)
THREADS_SRC  = $(addprefix $(THREADS_DIR)/, $(addsuffix .c, $(THREADS)))
THREADS_HDR  = $(addprefix $(THREADS_DIR)/, $(addsuffix .h, $(THREADS)))
THREADS_OBJ  = $(addprefix $(OBJ_DIR)/, 	$(addsuffix .o, $(THREADS)))
THREADS_BIN  = $(addprefix $(BIN_DIR)/, 	$(THREADS))
THREADS_ODEP = $(THREADS_SRC) $(THREADS_HDR)
THREADS_BDEP = $(THREADS_OBJ)


# target groups
# ==============================================================================
USER_ITEMS = THREADS
ALL_ITEMS  = $(USER_ITEMS)

EXPAND_GROUP = $(foreach item, $(patsubst %, %_$1, $2), $($(item)))

ALL_OBJS    = $(call EXPAND_GROUP,OBJ,$(ALL_ITEMS))
ALL_BINS    = $(call EXPAND_GROUP,BIN,$(ALL_ITEMS))
ALL_TARGETS = $(ALL_OBJS) $(ALL_BINS)


# make targets
# ==============================================================================
all: $(ALL_TARGETS)

$(THREADS_BIN): $(THREADS_BDEP)
	$(CC) $(CFLAGS) -o $@ $^

$(THREADS_OBJ): $(THREADS_ODEP)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(ALL_TARGETS)
