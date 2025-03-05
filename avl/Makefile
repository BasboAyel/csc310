INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := customErrorClass.o main.o avl.o
DEPS 		 := Includes/customErrorClass.h avl.h


.PHONY: clean all

all: customErrorClass.o main.o avl.o a

clean:
	rm $(OBJS) a

customErrorClass.o: customErrorClass.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

main.o: main.cpp ${DEPS}
	$(CC) -std=c++11 -c -o $@ $(CFLAGS) $<

avl.o: avl.cpp avl.h ${DEPS}
	$(CC) -std=c++11 -c -o $@ $(CFLAGS) $<


a: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)