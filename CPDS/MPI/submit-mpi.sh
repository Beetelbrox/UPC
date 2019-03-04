#!/bin/csh
# following option makes sure the job will run in the current directory
#$ -cwd
# following option makes sure the job has the same environmnent variables as the submission shell
#$ -V
# Canviar el nom del job
#$ -N lab1-mpi

mpirun.mpich -np 4 -machinefile -V $TMPDIR/machines ./heatmpi ./test.dat

