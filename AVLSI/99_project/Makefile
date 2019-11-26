CCC = g++
CFLAGS = -Wall -Wextra

FP_PROB = floorplanning_problem
FP_SOLVER = floorplanning_solver
FP_NPE = npe
FP = floorplan
TB = testbench

$(TB): $(TB).o $(FP_PROB).o  ${FP}.o $(FP_NPE).o $(FP_SOLVER).o
	$(CCC) $(CFLAGS) -o bin/$(TB) $(TB).o ${FP}.o $(FP_PROB).o $(FP_NPE).o $(FP_SOLVER).o

#$(TB): $(TB).o $(FP_SOLVER).o $(FP_PROB).o $(FP_NPE).o ${FP}.o
#	$(CCC) $(CFLAGS) -o bin/$(TB) $(TB).o ${FP}.o $(FP_NPE).o $(FP_PROB).o $(FP_SOLVER).o

${TB}.o: src/$(TB).cpp src/$(FP_SOLVER).h
	$(CCC) $(CFLAGS) -c src/$(TB).cpp

${FP_SOLVER}.o: src/$(FP_SOLVER).cpp src/$(FP_SOLVER).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_SOLVER).cpp

${FP_PROB}.o: src/$(FP_PROB).cpp src/$(FP_PROB).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_PROB).cpp

${FP}.o: src/$(FP).cpp src/$(FP).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP).cpp

${FP_NPE}.o: src/$(FP_NPE).cpp src/$(FP_NPE).h
	$(CCC) $(CFLAGS) $(INC) -c src/$(FP_NPE).cpp

clean:
	rm --force *.o bin/*
