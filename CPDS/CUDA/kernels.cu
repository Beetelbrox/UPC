#include <math.h>
#include <float.h>
#include <cuda.h>

__global__ void gpu_Heat (float *h, float *g, int N, float *residual) {
    extern __shared__ float v_reduction[]; // Vector to store the reduction values
    int block_id = (blockIdx.x + blockIdx.y*gridDim.x); 
    int t_id = block_id*blockDim.x*blockDim.y + threadIdx.x + threadIdx.y*blockDim.x;    
    int red_ix = threadIdx.x*blockDim.x + threadIdx.y;
    v_reduction[red_ix];
    // As we have created thread in excess, only proceed if the id of the thread is within the size of the matrix of elements to process.
    if(t_id < (N-2)*(N-2)){
      // As the area we have to process is smaller than u/uhelp and we have created threads in excess, let's do a
      // transformation to obtain the index of the position in u/uhelp tha the thread has to modify, so we can have cleaner code later on.
      int mod_id = N               // First Row of full matrix
                 + (t_id/(N-2))*N  // y-coord in small matrix times full row matrix
                 + 1               // First element of the row in the full matrix
                 + (t_id%(N-2));   // x-coord in small matrix
    
      g[mod_id] = 0.25*( h[mod_id-N]  // Up
                       + h[mod_id+1]  // Right
                       + h[mod_id+N]  // Down 
                       + h[mod_id-1]  // Left
                       );
      float diff = g[mod_id]-h[mod_id];
      v_reduction[red_ix] = diff*diff;
      
      __syncthreads();
    }
    
    // Reduction
    for(int s = (blockDim.x*blockDim.x)>>1; s>0; s>>=1){
      if(red_ix < s) v_reduction[red_ix] += v_reduction[red_ix+s];
      __syncthreads();
    }
    if(red_ix == 0) residual[blockIdx.x*gridDim.x+blockIdx.y] = v_reduction[0];
}
