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
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);

	matrix_p o;
	if (myrank == 0 ){
		o = matrix_construct( 1,5 );
		for ( int j = 0 ; j < 5 ; ++j ){
			double i =0;
			o -> vals[0][j] = i*5+j+1000;
		}
	} else if (myrank ==1 ){
		o = matrix_construct( 1,5 );
		for ( int j = 0 ; j < 5 ; ++j ){
			double i =0;
			o -> vals[0][j] = i*5+j+1000;
		}
	}else if (myrank == 2){
		o = matrix_construct( 1,5 );
		for ( int j = 0 ; j < 5 ; ++j ){
			double i =0;
			o -> vals[0][j] = i*5+j+1000;
		}
	}
	else if (myrank == 3){
		o = matrix_construct( 1,5 );
		for ( int j = 0 ; j < 5 ; ++j ){
			double i =0;
			o -> vals[0][j] = i*5+j+1000;
		}
	}
	else if (myrank == 4){
		o = matrix_construct( 1,5 );
		for ( int j = 0 ; j < 5 ; ++j ){
			double i =0;
			o -> vals[0][j] = i*5+j+1000;
		}
	}
	printf("process - %d", myrank);
	matrix_print(o);
	matrix_p p = transpose(o);
	//printf("transposed\n!");
	//matrix_print(p);
	MPI_Finalize();
	return 0;
}
