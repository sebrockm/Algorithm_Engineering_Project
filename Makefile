CC = g++-4.7
CFLAGS = -c -Wall -std=c++11 -O3 -pg
LDFLAGS = -std=c++11 -O3 -pg
SOURCES = main.cpp Heuristic1.cpp crossing.cpp Label.cpp KDTree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
OUT = pflp_solver

all: $(SOURCES) $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(OUT)
