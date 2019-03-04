#!/bin/csh
# following option makes sure the job will run in the current directory
#$ -cwd
# following option makes sure the job has the same environmnent variables as the submission shell
#$ -V
# Canviar el nom del job
#$ -N lab1-cuda

setenv  LD_LIBRARY_PATH /Soft/cuda/9.0.176/lib64/

./heatCUDA ./test.dat -t 16 -b 32

