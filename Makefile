CC = g++-4.7
CFLAGS = -c -Wall -std=c++11
LDFLAGS = -std=c++11
SOURCES = main.cpp heuristic1.cpp crossing.cpp Label.cpp
OBJECTS = $(SOURCES:.cpp=.o)
OUT = pflp_solver

all: $(SOURCES) $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(OUT)
