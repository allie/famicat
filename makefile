CC=gcc
CFLAGS=-std=c89 -c -Wall -D_POSIX_C_SOURCE=200112L -DDEBUG_MODE -g
LDFLAGS=-lm
SOURCES=$(shell find src -name "*.c")
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=famicat
OS=$(shell gcc -dumpmachine)

ifneq (, $(findstring mingw, $(OS)))
	LDFLAGS := -lmingw32 $(LDFLAGS)
endif

all: clean $(SOURCES) $(EXECUTABLE)

debug: CFLAGS += -D__DEBUG__
debug: all

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE)
	find src -name "*.o" -print0 | xargs -0 rm -rf
