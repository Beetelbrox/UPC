CCC = g++
CFLAGS = -Wall

FP_PROB = floorplanning_problem
FP_SOLVER = floorplanning_solver
FP_SOLUTION = floorplanning_solution
FP_NPE = npe
TB = testbench

$(TB): $(TB).o $(FP_PROB).o $(FP_SOLVER).o $(FP_SOLUTION).o $(FP_NPE).o
	$(CCC) $(CFLAGS) -o bin/$(TB) $(TB).o $(FP_PROB).o $(FP_SOLVER).o $(FP_SOLUTION).o $(FP_NPE).o

${TB}.o: src/$(TB).cpp src/$(FP_PROB).h src/$(FP_SOLVER).h src/$(FP_SOLUTION).h src/$(FP_NPE).h
	$(CCC) $(CFLAGS) -c src/$(TB).cpp

${FP_SOLVER}.o: src/$(FP_SOLVER).cpp src/$(FP_SOLVER).h src/$(FP_PROB).h src/$(FP_SOLUTION).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_SOLVER).cpp

${FP_SOLUTION}.o: src/$(FP_SOLUTION).cpp src/$(FP_SOLUTION).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_SOLUTION).cpp

${FP_NPE}.o: src/$(FP_NPE).cpp src/$(FP_NPE).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_NPE).cpp

${FP_PROB}.o: src/$(FP_PROB).cpp src/$(FP_PROB).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_PROB).cpp

clean:
	rm --force *.o
