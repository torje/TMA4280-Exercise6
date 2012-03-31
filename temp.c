#include <mpi.h>
#include "matrix.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
int nprocs, problemsize, width, myrank, namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;

void matrix_print(matrix_p a){
	for ( int i =0; i <a->width; ++i ){
		printf("[ ");
		for ( int j = 0 ; j < a->depth; ++j){
			printf(" %.0lf " , a->vals[i][j]);
		}
		printf(" ]\n");
	}
}
Real inhabitant(int i , int  j, Real scale){
	Real _i = i;
	Real _j = j;
	//return scale*sin(M_PI*_i*scale)*sin(M_PI*_i*scale);
	return _i*5+j;
}
int main(int argc, char **argv){
	problemsize = 25;

	MPI_Init( &argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);
	matrix_p o = Gen_matrix( problemsize, nprocs, myrank, &inhabitant);
	sleep(myrank);
	matrix_print(o);
	sleep(5);
	if (myrank == 0) printf("-------------------------------------\n");
	matrix_p p = transpose(o);
	sleep(myrank);
	matrix_print(p);


	MPI_Finalize();
	return 0;
}
