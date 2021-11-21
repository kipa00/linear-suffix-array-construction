CC=g++
CFLAGS=-c -O2 -fPIC -std=gnu++14
TEST_OBJS=bitset.o sais.o skew.o test.o util.o
SHARED_OBJS=bitset.o sais.o shared.o skew.o util.o

all: test shared

test: $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $@ -O2

shared: salib.so
	$(NOECHO) $(NOOP)

salib.so: $(SHARED_OBJS)
	$(CC) $(SHARED_OBJS) -shared -o $@ -O2

bitset.o: bitset.cpp common.h bitset.h
	$(CC) bitset.cpp -o $@ $(CFLAGS)

sais.o: sais.cpp common.h util.h bitset.h sais.h
	$(CC) sais.cpp -o $@ $(CFLAGS)

shared.o: shared.cpp common.h sais.h skew.h
	$(CC) shared.cpp -o $@ $(CFLAGS)

skew.o: skew.cpp util.h skew.h common.h
	$(CC) skew.cpp -o $@ $(CFLAGS)

test.o: test.cpp common.h skew.h sais.h
	$(CC) test.cpp -o $@ $(CFLAGS)

util.o: util.cpp util.h common.h
	$(CC) util.cpp -o $@ $(CFLAGS)

clean:
	rm -f *.o test salib.so