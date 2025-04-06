CC?=gcc
DBG?=gdb

TARGET?=debug

NAME:=tcom
VMAJ:=0
VMIN:=1
VREV:=0

IDIRS:=-Iinclude/
CDEFS:=-D_FILE_OFFSET_BITS=64 -DTARGET=$(TARGET)
CDEFS+=-DNAME=\"$(NAME)\" -DVMAJ=$(VMAJ) -DVMIN=$(VMIN) -DVREV=$(VREV)

IDIRS_APP:=$(IDIRS) -Icli/
IDIRS_LIB:=$(IDIRS) -Ilib/

CFLAGS+=$(CDEFS) -fstack-protector-strong -Wall -Wextra -Werror -MMD -c -fpie 
LFLAGS+=-pie -fstack-protector-strong

ifeq ($(TARGET), debug)
CFLAGS+=-Og -g
else ifeq ($(TARGET), release)
CFLAGS+=-O2
else
$(error "unknown target $(TARGET)")
endif

DEPS:=libunwind

CFLAGS_DEPS:=$(foreach dep, $(DEPS), $(shell pkg-config --cflags $(dep)))
LFLAGS_DEPS:=$(foreach dep, $(DEPS), $(shell pkg-config --libs $(dep)))

CSRC_APP:=$(shell find cli/ -type f -regex '.*\.c')
COBJ_APP:=$(patsubst cli/%.c, build/cli/%.c.o, $(CSRC_APP))

CSRC_LIB:=$(shell find lib/ -type f -regex '.*\.c')
COBJ_LIB:=$(patsubst lib/%.c, build/lib/%.c.o, $(CSRC_LIB))

BIN:=build/$(NAME)
LIB:=build/lib$(NAME).a

# build library
build/lib/%.c.o: lib/%.c
	mkdir -p $(dir $@)
	$(CC) $(IDIRS_LIB) $(CFLAGS) $< -o $@

$(LIB): $(COBJ_LIB)
	ar -rcs $@ $(COBJ_LIB)

# build binary 
build/cli/%.c.o: cli/%.c
	mkdir -p $(dir $@)
	$(CC) $(IDIRS_APP) $(CFLAGS) $(CFLAGS_DEPS) $< -o $@

$(BIN): $(COBJ_APP) $(LIB)
	$(CC) $(LFLAGS) $(COBJ_APP) $(LIB) -lutf $(LFLAGS_DEPS) -o $@

.PHONY: all run debug clean bear
.DEFAULT_GOAL:=all

all: $(BIN)
lib: $(LIB)

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	$(DBG) $(BIN)

clean: 
	rm -rf build/

bear: 
	./util/bear.sh

-include build/*.c.d
