CC = g++-4.7
CFLAGS = -c -Wall -std=c++11 -O3 -pg
LDFLAGS = -std=c++11 -O3 -pg
SOURCES = main.cpp Heuristic2.cpp Heuristic1.cpp crossing.cpp Label.cpp KDTree.cpp
SCIPLOC = scipoptsuite-3.0.2/scip-3.0.2/
INCLFLAGS = -I$(SCIPLOC)src
LIBFLAGS = -I$(SCIPLOC)lib
OBJECTS = $(SOURCES:.cpp=.o)
OUT = pflp_solver

all: $(SOURCES) $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LIBFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLFLAGS) $< -o $@

clean:
	rm -f *.o $(OUT)
