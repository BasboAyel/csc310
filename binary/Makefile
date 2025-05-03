INCLUDE_DIRS := .
CC           := g++
CFLAGS       := -I$(INCLUDE_DIRS)

OBJS := mainSamples.o geneBank.o customErrorClass.o customErrorHandling.o
DEPS := $(INCLUDE_DIRS)/customErrorClass.h $(INCLUDE_DIRS)/geneBank.h

.PHONY: all clean

all: a

a: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

mainSamples.o: mainSamples.cpp $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

geneBank.o: geneBank.cpp $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

customErrorClass.o: customErrorClass.cpp $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

customErrorHandling.o: customErrorHandling.cpp $(DEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(OBJS) a
