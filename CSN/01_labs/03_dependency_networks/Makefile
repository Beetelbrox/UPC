CCC = g++
CFLAGS = -Wall

GRAPH = graph
SUMMARY = generate_summary
NOOPT = montecarlo_no_optimization

$(NOOPT): $(NOOPT).o $(GRAPH).o
	$(CCC) $(CFLAGS) -o bin/$(NOOPT) $(NOOPT).o $(GRAPH).o

${NOOPT}.o: src/$(NOOPT).cpp src/$(GRAPH).h
	$(CCC) $(CFLAGS) -c src/$(NOOPT).cpp

$(SUMMARY): $(SUMMARY).o $(GRAPH).o
	$(CCC) $(CFLAGS) -o bin/$(SUMMARY) $(SUMMARY).o $(GRAPH).o

${SUMMARY}.o: src/$(SUMMARY).cpp src/$(GRAPH).h
	$(CCC) $(CFLAGS) -c src/$(SUMMARY).cpp


${GRAPH}.o: src/$(GRAPH).cpp src/$(GRAPH).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(GRAPH).cpp

clean:
	rm --force *.o
