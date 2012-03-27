#include <stdlib.h>
#include <string.h>
#include "matrix.h"

matrix_p matrix_construct(int width, int depth){
	matrix_p matrix = malloc(sizeof(matrix_t)+sizeof(Real*)*width + sizeof(Real)*depth);
	for ( int i = 0 ;  i < matrix -> width ; ++i){
		matrix -> vals [i] = (Real*)&(matrix->vals) + 1 +i*width*sizeof(Real);
	}
	return matrix;
}

void matrix_delete(matrix_p old){
	free( old );
	return;
}
Real * matrix_sort(matrix_p old, int nprocs){
	Real * sorted = malloc( sizeof(Real)*old -> depth * old-> width);
	Real * ptr = sorted;
	int stride;
	int offset = 0;
	for ( int j = 0 ; j < old -> depth ; ++j){
		stride = old -> depth/nprocs;
		stride += (j <  old->depth / nprocs ) ? 1 : 0 ;
		for ( int i = 0 ; i < old -> width ; ++i ){
			memcpy( ptr , &(old -> vals[i][0])+offset,  stride*sizeof(Real));
			ptr += stride;
		}
		offset+=stride;
	}
	return sorted;
}
int* create_senddispl( int size , int nprocs) {
	int * sizes = malloc( sizeof( Real ) * nprocs );
	sizes[0] = 0;
	{
		int i;
		int block;
		block = size / nprocs +1;
		for (i = 1 ; i < size %nprocs ; i++) {
			sizes[i] = sizes[i-1]+block;
		}
		block = size / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = sizes[i-1]+block;
		}
	}
	return sizes; 
}

int* matrix_helper( int size , int nprocs) {
	int * sizes = malloc( sizeof( Real ) * nprocs );
	{
		int i;
		int block;
		block = size / nprocs +1;
		for (i = 0 ; i < size %nprocs ; i++) {
			sizes[i] = block;
		}
		block = size / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = block;
		}
	}
	return sizes; 
}

int* processlist(int nprocs){
	int * processlist = malloc(sizeof(int)*nprocs);
	for( int i = 0 ; i < nprocs ; ++i){
		processlist[i] = i;
	}
	return processlist;
}
comm_helper_p create_comm_list(int nprocs, int problemsize, matrix_p data){
	comm_helper_p comms = malloc(sizeof(comm_helper_t));
	comms -> nprocs = nprocs;
	comms -> problemsize = problemsize;
	comms -> range = processlist(nprocs);
	comms -> partitions = matrix_helper(problemsize, nprocs);
	comms -> data= matrix_sort(data, nprocs);
	comms -> senddispl= create_senddispl(problemsize, nprocs);
	return comms;
}
void free_comm_list(comm_helper_p a){
    free(a -> range);
    free(a -> partitions);
    free(a -> data);
	free(a);
	return;
}
matrix_p sendarr( comm_helper_p a){
	int width = a -> problemsize / nprocs + (a -> problemsize %nprocs < myrank)?1: 0;
	int depth = a -> problemsize;
	int recvsize = width * depth;
	Real *recvbuf = malloc(sizeof(Real)*recvsize);
	MPI_Alltoallv( a -> data ,a -> partitions, a -> senddispl, MPI_DOUBLE, recvbuf);
}

int * receivecounts(int nprocs, int dim){
	int *  recv = malloc(sizeof(int)*nprocs);
	int *i;
	for (i = recv ; i < recv+(dim %nprocs) ; ++i){
		i = problemsize / nprocs + 1;
	}
	for (		; i < recv + nprocs ; ++i){
		i = problemsize / nprocs ;
	}
	return recv;
}

