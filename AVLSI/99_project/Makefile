CCC = g++
CFLAGS = -Wall -Wextra

FP_PROB = floorplanning_problem
FP_SOLVER = floorplanning_solver
FP_NPE = npe
FP_SOL = floorplanning_solution
SF = shape_function
TB = testbench

$(TB): $(TB).o $(FP_PROB).o  $(FP_NPE).o $(FP_SOLVER).o $(FP_SOL).o $(SF).o
	$(CCC) $(CFLAGS) -o bin/$(TB) $(TB).o $(FP_PROB).o $(FP_NPE).o $(FP_SOLVER).o $(FP_SOL).o $(SF).o

${TB}.o: src/$(TB).cpp src/$(FP_SOLVER).h
	$(CCC) $(CFLAGS) -c src/$(TB).cpp

${FP_SOLVER}.o: src/$(FP_SOLVER).cpp src/$(FP_SOLVER).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_SOLVER).cpp

${FP_PROB}.o: src/$(FP_PROB).cpp src/$(FP_PROB).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_PROB).cpp

${FP_SOL}.o: src/$(FP_SOL).cpp src/$(FP_SOL).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_SOL).cpp

${SF}.o: src/$(SF).cpp src/$(SF).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(SF).cpp

${FP_NPE}.o: src/$(FP_NPE).cpp src/$(FP_NPE).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_NPE).cpp

clean:
	rm --force *.o bin/*
