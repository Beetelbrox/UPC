CCC = g++
CFLAGS = -Wall

GRAPH = graph
SUMMARY = generate_summary
MAIN = generate_summary

$(SUMMARY): $(SUMMARY).o $(GRAPH).o
	$(CCC) $(CFLAGS) -o bin/$(SUMMARY) $(SUMMARY).o $(GRAPH).o

${SUMMARY}.o: src/$(SUMMARY).cpp src/$(GRAPH).h
	$(CCC) $(CFLAGS) -c src/$(SUMMARY).cpp


${GRAPH}.o: src/$(GRAPH).cpp src/$(GRAPH).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(GRAPH).cpp

clean:
	rm --force *.o
