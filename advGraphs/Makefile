INCLUDE_DIRS := Includes
CC           := g++
CFLAGS       := -I$(INCLUDE_DIRS)

OBJS         := main.o binaryHeap.o binomialHeap.o customErrorHandling.o customErrorClass.o mst.o
DEPS         := ${INCLUDE_DIRS}/customErrorClass.h binaryHeap.h binomialHeap.h mst.h

.PHONY: clean all

all: $(OBJS) a

clean:
	rm -f $(OBJS) a

main.o: main.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

binaryHeap.o: binaryHeap.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

binomialHeap.o: binomialHeap.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

 

mst.o: mst.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a:$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)




