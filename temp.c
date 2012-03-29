#include <mpi.h>
#include "matrix.h"
#include <stdio.h>
#include <unistd.h>
int nprocs, problemsize, width, myrank, namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;

void matrix_print(matrix_p a){
	for ( int i =0; i <a->width; ++i ){
		printf("[ ");
		for ( int j = 0 ; j < a->depth; ++j){
			printf(" %lf " , a->vals[i][j]);
		}
		printf(" ]\n");
	}
}
int main(int argc, char **argv){
	problemsize = 5;
	
	MPI_Init( &argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	printf("%d\n", nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);

	Real n  = 0.;
	matrix_p o = matrix_construct( 5 ,5 );
	for (int i = 0 ; i < 5 ; ++i){
		for ( int j = 0 ; j < 5 ; ++j ){
			o -> vals[i][j] = n;
			n+=1;
		}
	}
	matrix_print(o);
	matrix_p p = transpose(o);
	printf("transposed\n!");
	matrix_print(p);
	MPI_Finalize();
	return 0;
}

