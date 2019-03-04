/*
 * Iterative solver for heat distribution
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "heat.h"

void usage( char *s )
{
    fprintf(stderr, 
	    "Usage: %s <input file> [result file]\n\n", s);
}

int main( int argc, char *argv[] ){
    unsigned iter;
    FILE *infile, *resfile;
    char *resfilename;
    int myid, numprocs;
    int rows, nrows;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0) {
      printf("I am the master (%d) and going to distribute work to %d additional workers ...\n", myid, numprocs-1);

      // algorithmic parameters
      algoparam_t param;
      int np;

      double runtime, flop;
      double residual=0.0;

// ---------------------------------------------------
// ARGUMENTS & INTIALIZATION
// ---------------------------------------------------

      // check arguments
      if( argc < 2 ){
	usage( argv[0] );
	return 1;
      }

      // check input file
      if( !(infile=fopen(argv[1], "r"))  ){
        fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);
	usage(argv[0]);
	return 1;
      }

      // check result file
      resfilename= (argc>=3) ? argv[2]:"heat.ppm";

      if( !(resfile=fopen(resfilename, "w")) ){
	fprintf(stderr,"\nError: Cannot open \"%s\" for writing.\n\n", resfilename);
	usage(argv[0]);
	return 1;
      }

      // check input
      if( !read_input(infile, &param)){
	fprintf(stderr, "\nError: Error parsing input file.\n\n");
	usage(argv[0]);
	return 1;
      }
      print_params(&param);

      // set the visualization resolution
    
      param.u     = 0;
      param.uhelp = 0;
      param.uvis  = 0;
      param.visres = param.resolution;
   
      if( !initialize(&param)){
	    fprintf(stderr, "Error in Solver initialization.\n\n");
	    usage(argv[0]);
            return 1;
      }

//---------------------------------------------------
// THREAD COUNT CALCULATION      
//---------------------------------------------------
      if(param.resolution % numprocs != 0){
        printf("Error: Number of treads (%d) has to divide the number of rows (%d)\n", numprocs, param.resolution);
        MPI_Abort(MPI_COMM_WORLD, -1); // Terminate the parallelization if wrong num of threads
      }
      printf("Number of Threads: %d\n", numprocs); 


      // full size (param.resolution are only the inner points)
      np = param.resolution + 2;
    
      // starting time
      runtime = wtime();

//---------------------------------------------------
// MPI STARTS HERE
//---------------------------------------------------
      // Compute the number of rows to be sent to each worker thread
      rows = param.resolution/numprocs;
      nrows = rows + 2;

      // Broadcast the data common to all workers
      MPI_Bcast(&param.maxiter, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(&param.resolution, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(&param.algorithm, 1, MPI_INT, 0, MPI_COMM_WORLD);
      
      // send to workers the necessary data to perform computation
      for (int i=1; i<numprocs; i++) { 
        MPI_Send(&param.u[i*np*rows], nrows*np, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        MPI_Send(&param.uhelp[i*np*rows], nrows*np, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }

      iter = 0;
      while(1) {
        switch( param.algorithm ) {
          case 0: // JACOBI
            residual = relax_jacobi(param.u, param.uhelp, nrows, np);
	    // Copy uhelp into u
            for (int i=0; i<nrows; i++)
              for (int j=0; j<np; j++)
                param.u[ i*np+j ] = param.uhelp[ i*np+j ];
            // If we have more than one thread, send the last row to the next one
            if(numprocs > 1){
              // Send the first row to the next thread
              MPI_Send(&param.u[np*rows], np, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD);
              // Receive the last row from the next thread
              MPI_Recv(&param.u[np*(rows+1)], np, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD, &status);
            }   
            break;
	  case 1: // RED-BLACK
            residual = relax_redblack(param.u, np, np);
            break;
  	  case 2: // GAUSS
            residual = relax_gauss(param.u, np, np);
          break;
	}
        iter++;
        // We need to reduce with Allreduce so all threads know that they have converged to the
        // required error and can stop their execution
        MPI_Allreduce(MPI_IN_PLACE, &residual, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // solution good enough ?
        if (residual < 0.00005) break;

        // max. iteration reached ? (no limit with maxiter=0)
        if (param.maxiter>0 && iter>=param.maxiter) break;
    }

    // Once we have obtained a solution, copy it back to the main matrix
    for(int i=1; i<numprocs; i++){
      MPI_Recv(&param.u[(1+rows*i)*np], rows+np, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status); 
    }

    // Flop count after iter iterations
    flop = iter * 11.0 * param.resolution * param.resolution;
    // stopping time
    runtime = wtime() - runtime;

    fprintf(stdout, "Time: %04.3f ", runtime);
    fprintf(stdout, "(%3.3f GFlop => %6.2f MFlop/s)\n", 
	    flop/1000000000.0,
	    flop/runtime/1000000);
    fprintf(stdout, "Convergence to residual=%f: %d iterations\n", residual, iter);

    // for plot...
    coarsen( param.u, np, np,
	     param.uvis, param.visres+2, param.visres+2 );
  
    write_image( resfile, param.uvis,  
		 param.visres+2, 
		 param.visres+2 );

    finalize( &param );

    fprintf(stdout, "Process %d finished computing with residual value = %f\n", myid, residual);
    
    // Wait until all process have finished printing their messages
    if(numprocs > 1) MPI_Barrier(MPI_COMM_WORLD);
   
    MPI_Finalize();

    return 0;

} else {

    printf("I am worker %d and ready to receive work to do ...\n", myid);

    // receive information from master to perform computation locally

    int columns, rows, np;
    int iter, maxiter;
    int algorithm;
    double residual;

    MPI_Recv(&maxiter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&columns, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&algorithm, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    
    np = columns + 2;
    // Get the amount of rows this thread has to iterate through
    rows = columns/numprocs;
    nrows = rows+2;

    // allocate memory for worker
    double * u = calloc( sizeof(double), nrows*np );
    double * uhelp = calloc( sizeof(double), nrows*np );
    if( (!u) || (!uhelp) ){
        fprintf(stderr, "Error: Cannot allocate memory\n");
        return 0;
    }
    
    // fill initial values for matrix with values received from master
    MPI_Recv(&u[0], nrows*np, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&uhelp[0], nrows*np, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

    iter = 0;
    MPI_Request request;
    while(1) {
      switch( algorithm ) {
        case 0: // JACOBI
          residual = relax_jacobi(u, uhelp, nrows, np);
          // Copy uhelp into u
          for (int i=0; i<nrows; i++)
            for (int j=0; j<np; j++)
	      u[ i*np+j ] = uhelp[ i*np+j ];
          // If we are not the last thread, send & receive upwards
          if(myid < numprocs-1){
            
            // Non-Blocking send the last row to the next thread and blocking wait for the last row of the
            // previous, then wait for the send to be completed
            MPI_Isend(&u[np*rows], np, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD, &request); 
            MPI_Recv(&u[0], np, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD, &status);
            MPI_Wait(&request, &status);
            
            // Same as before but in reverse
            MPI_Isend(&u[np], np, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD, &request);
            MPI_Recv(&u[np*(rows+1)], np, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD, &status);
            MPI_Wait(&request, &status);
          // If we are the last thread we first receive and then send to avoid deadlocks
          }else{
            MPI_Recv(&u[0], np, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&u[np], np, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD);
          }
	  break;
	    case 1: // RED-BLACK
		    residual = relax_redblack(u, np, np);
		    break;
	    case 2: // GAUSS
		    residual = relax_gauss(u, np, np);
		    if(numprocs > 1)
                      MPI_Recv(&u[(rows+1)*np], np, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD, &status);
                    break;
	    }

        iter++;
        // Reduce and synchronize residual to see if we have finished iterating.
        MPI_Allreduce(MPI_IN_PLACE, &residual, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // solution good enough ?
        if (residual < 0.00005) break;

        // max. iteration reached ? (no limit with maxiter=0)
        if (maxiter>0 && iter>=maxiter) break;
    }
    
    MPI_Send(&u[np], np*rows, MPI_DOUBLE,0 ,0, MPI_COMM_WORLD); 

    if( u ) free(u); if( uhelp ) free(uhelp);

    fprintf(stdout, "Process %d finished computing %d iterations with residual value = %f\n", myid, iter, residual);
    
    // Barrier to make sure all threads are done
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    exit(0);
  }
}
