CC=clang
CFLAGS=-std=c99 -c -Wall -D_POSIX_C_SOURCE=200112L -DDEBUG_MODE -DUNOFFICIAL_MODE -g
LDFLAGS=-lSDL2_image -lSDL2 -lm
SOURCES=$(shell find src -name "*.c")
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=famicat
OS=$(shell gcc -dumpmachine)

ifneq (, $(findstring mingw, $(OS)))
	LDFLAGS := -lmingw32 -lSDL2main $(LDFLAGS)
	CC=gcc
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
