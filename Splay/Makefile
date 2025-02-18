INCLUDE_DIRS := Includes
CC           := g++
CFLAGS       := -I$(INCLUDE_DIRS)

OBJS         := customErrorClass.o splay.o mainsplay.o
DEPS         := ${INCLUDE_DIRS}/customErrorClass.h splay.h

.PHONY: clean all

all: customErrorClass.o splay.o mainsplay.o a

clean:
	# Use a tab here, and remove the stray period (.)
	rm $(OBJS) a

customErrorClass.o: customErrorClass.cpp ${DEPS}
	# Use a tab here!
	$(CC) -c -o $@ $(CFLAGS) $<

splay.o: splay.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mainsplay.o: mainsplay.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
