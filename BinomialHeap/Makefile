INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := main.o binomial.o customErrorClass.o 
DEPS 		 := ${INCLUDE_DIRS}/customErrorClass.h binomial.h

.PHONY: clean all

all: customErrorClass.o binomial.o main.o a

clean:
	rm $(OBJS) a

binomial.o: binomial.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

main o: main.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)