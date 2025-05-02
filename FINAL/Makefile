INCLUDE_DIRS := .
CC           := g++
CFLAGS       := -I$(INCLUDE_DIRS) -std=c++11

OBJS         := heap.o hashtable.o avltree.o splaytree.o snake.o
DEPS         := heap.h hashtable.h avltree.h splaytree.h

.PHONY: clean all

all: snakeGame

snake.o: snake.cpp $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

heap.o: heap.cpp heap.h
	$(CC) -c -o $@ $(CFLAGS) $<

hashtable.o: hashtable.cpp hashtable.h
	$(CC) -c -o $@ $(CFLAGS) $<

avltree.o: avltree.cpp avltree.h
	$(CC) -c -o $@ $(CFLAGS) $<

splaytree.o: splaytree.cpp splaytree.h
	$(CC) -c -o $@ $(CFLAGS) $<

snakeGame: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) snakeGame
