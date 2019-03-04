NVCC 	= /Soft/cuda/9.0.176/bin/nvcc
CUDAPATH = /Soft/cuda/9.0.176/

NVCCFLAGS = -I$(CUDAPATH)/include #--ptxas-options=-v 
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm

ALL	= heatCUDA
all: $(ALL)

kernels.o: kernels.cu 
	$(NVCC) -c -g $(NVCCFLAGS) $+ $(LFLAGS) -o $@

heatCUDA: heatCUDA.cu kernels.o
	$(NVCC) -g -O2 $(NVCCFLAGS) $+ $(LFLAGS) -o $@

clean:
	rm -fr $(ALL) *.o *.prv *.pcf *.row *.sym *.mpits set-0

