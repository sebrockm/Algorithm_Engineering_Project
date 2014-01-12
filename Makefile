CC = g++-4.7
CFLAGS = -c -Wall -std=c++11 -O3 -pg
LDFLAGS = -std=c++11 -O3 -pg
SOURCES = main.cpp Heuristic2.cpp Heuristic1.cpp crossing.cpp Label.cpp KDTree.cpp Solver.cpp
SCIPLOC = scipoptsuite-3.0.2/scip-3.0.2/
INCLFLAGS = -I$(SCIPLOC)src
LIBFLAGS = -L$(SCIPLOC)lib -lscip -lobjscip -lnlpi.cppad -llpispx -lsoplex.linux.x86_64.gnu.opt -lz -lreadline
OBJECTS = $(SOURCES:.cpp=.o)
OUT = pflp_solver

all: $(SOURCES) $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBFLAGS) 

.cpp.o: $(SOURCES)
	$(CC) $(INCLFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(OUT)

test:
	./pflp_solver -in $(IN) -out test.dat -heu 0 && ./pflp_solver -eval test.dat

