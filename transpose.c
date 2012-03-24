#include <omp.h>
#include <openmp.h>

void PARALLELL_transpose(matrix_p send, matrix_p receive){
	{
		int i;
		int sendcount;
		sendcount = dim/procs;
		for ( i = 0; i < a -> width ; ++i){
			int j;
			for ( j = 0 ; a -> vals[i] -> ; j+=sendcount+1){
				MPI_Isend( a[i] -> vals[j], sendcount+1);
			}
			for (       ; a -> vals[i] -> ; j+=sendcount){
				MPI_Isend( a[i] -> vals[j], sendcount );
			}
		}
	}
	MPI_Request *requests =  malloc(sizeof(MPI_Request)*dim
	for ( i = 0; i < b -> width ; ++i){
		receive_count = dim%procs > myrank ?  dim/procs + 1 : dim/procs;

		for ( int sender = 0 ; sender < receive -> size ; sender++, pos += receive_count){
			MPI_Irecv( &(b[i] -> vals[pos]), receive_count,sender);
		}
	}
}
