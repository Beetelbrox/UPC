CC     =  icc 
CC     =  gcc 

OPT3 	= -O3
CFLAGS  = -Wall -std=c99
LFLAGS  = -lm 

ALL	= heat heat-omp 
all: $(ALL)

misc.o: misc.c
	$(CC) -c $(CFLAGS) $(OPT3) $< -o $@

heat: heat-omp.c solver-omp.c misc.o
	$(CC) $(CFLAGS) $(OPT3) $+ $(LFLAGS) -o $@

heat-omp: heat-omp.c solver-omp.c misc.o
	$(CC) $(CFLAGS) $(OPT3) -fopenmp $+ $(LFLAGS) -o $@

clean:
	rm -fr $(ALL) heat.ppm *.o *.out *.err *.prv *.pcf *.row *.sym TRACE.mpits set-0 submit-omp.sh.o* submit-omp.sh.e* submit-omp-i.sh.o* submit-omp-i.sh.e*

ultraclean:
	rm -fr $(ALL) *.o *.ppm .tareador_precomputed* *.prv *.pcf *.row dependency_graph* *.times.txt lab1-omp* *.ps *.txt *.log TRACE.* set-0 logs
