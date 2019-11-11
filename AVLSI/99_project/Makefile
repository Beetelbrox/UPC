CCC = g++
CFLAGS = -Wall

FP_PROB = floorplanning_problem
TB = testbench

$(TB): $(TB).o $(FP_PROB).o
	$(CCC) $(CFLAGS) -o bin/$(TB) $(TB).o $(FP_PROB).o

${TB}.o: src/$(TB).cpp src/$(FP_PROB).h
	$(CCC) $(CFLAGS) -c src/$(TB).cpp


${FP_PROB}.o: src/$(FP_PROB).cpp src/$(FP_PROB).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_PROB).cpp

clean:
	rm --force *.o
