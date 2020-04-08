VPATH=src/:include/
CC=gcc
INCLUDE=include/
CFLAGS=-I $(INCLUDE) -Wall -Werror
CC_CMD=$(CC) $(CFLAGS)
LIB=-lpthread

OBJ_DIR=obj
BIN_DIR=bin
DEP_DIR=deps

SRCS=$(wildcard *.c)
DEPS=$(srcs:.c=.d)

all: dir_tgt rainbow

rainbow: connection.o parser.o task.o interpreter.o main.o
	$(CC_CMD) $(^:%.o=$(OBJ_DIR)/%.o) -o $(BIN_DIR)/$@ $(LIB)

%.o : %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $(OBJ_DIR)/$@

.PHONY: clean obj_dir_tgt bin_dir_tgt dep_dir_tgt

MKDIR_P=mkdir -p

dir_tgt: obj_dir_tgt bin_dir_tgt dep_dir_tgt

obj_dir_tgt:
	$(MKDIR_P) $(OBJ_DIR)

bin_dir_tgt:
	$(MKDIR_P) $(BIN_DIR)

dep_dir_tgt:
	$(MKDIR_P) $(DEP_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)

-include $(DEPS)
