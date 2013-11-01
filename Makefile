CC = g++-4.7
CFLAGS = -Wall
OUT = beschrift

all: main.o
	$(CC) main.o -o $(OUT) $(CFLAGS)

main.o: main.cpp
	$(CC) main.cpp -c $(CFLAGS)

clean:
	rm -f *.o $(OUT)
