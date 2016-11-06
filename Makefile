CC=gcc
CFLAGS=-c -Wall -std=gnu99
LDFLAGS=-Iinclude
SOURCES=src/main.c src/jorg.c src/character.c src/command.c src/situation.c src/item.c src/class.c src/spell.c src/creature.c src/save.c src/book.c
OBJECTS=$(SOURCES:.c=.o)
OBJDIR=obj
EXECUTABLE=bin/jorg

all: $(SOURCES) $(EXECUTABLE)

install:
	install -m 755 $(EXECUTABLE) /usr/local/bin

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lm

clean:
	find . -name "*.o" -delete

move:
	mv $(OBJECTS) $(OBJDIR)

.c.o:
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@
