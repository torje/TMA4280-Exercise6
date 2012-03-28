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
Real * matrix_sort(matrix_p old){
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
matrix_p matrix_unsort(Real * data){
	int width = problemsize / nprocs +  ((problemsize %nprocs < myrank)?1: 0);
	matrix_p matrix = matrix_construct( width , problemsize);
	for ( int i = 0 ; i < width ; ++i){
		for ( int j = 0 ; j < problemsize ; ++j){
			matrix -> vals[i][j] = data[i*width + j];
		}
	}
	free(data);
	return matrix;
}
int* create_senddispl() {
	int * sizes = malloc( sizeof( Real ) * nprocs );
	sizes[0] = 0;
	{
		int i;
		int block;
		block = problemsize / nprocs +1;
		for (i = 1 ; i < problemsize %nprocs ; i++) {
			sizes[i] = sizes[i-1]+block;
		}
		block = problemsize / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = sizes[i-1]+block;
		}
	}
	return sizes; 
}

int* c_sendcounts() {
	int * sizes = malloc( sizeof( Real ) * nprocs );
	{
		int i;
		int block;
		block = problemsize / nprocs +1;
		for (i = 0 ; i < problemsize %nprocs ; i++) {
			sizes[i] = block;
		}
		block = problemsize / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = block;
		}
	}
	return sizes; 
}

comm_helper_p create_comm_list(matrix_p data){
	comm_helper_p comms = malloc(sizeof(comm_helper_t));
	comms -> sendcounts = c_sendcounts();
	comms -> data= matrix_sort(data);
	comms -> senddispl= create_senddispl();
	comms -> recvdispl = c_recvdispl();
	comms -> recvcounts = c_receivecounts();
	return comms;
}
void free_comm_list(comm_helper_p a){
    free( a ->sendcounts);
    free( a ->senddispl);
	free( a ->recvcounts);
	free( a ->recvdispl);
	free( a ->data);
	free(a);
	return;
}
Real *sendarr( comm_helper_p a){
	int width = problemsize / nprocs +  ((problemsize %nprocs < myrank)?1: 0);
	int depth = problemsize;
	int recvsize = width * depth;
	Real *recvbuf = malloc(sizeof(Real)*recvsize);
	MPI_Alltoallv( a -> data ,a -> senddispl, a -> senddispl, MPI_DOUBLE, recvbuf, a->recvcounts, a->recvdispl , MPI_DOUBLE, world_com);
	return recvbuf;
}

int * c_receivecounts(){
	int *  recv = malloc(sizeof(int)*nprocs);
	int recvsize = problemsize / nprocs +( ( problemsize % nprocs) >= myrank) ? 1:0;
	for (int *i = recv ; i < recv + nprocs ; ++i){
		*i = recvsize;
	}
	return recv;
}

int *c_recvdispl(){
	int *  recv = malloc(sizeof(int)*nprocs);
	int recvsize = problemsize / nprocs +( ( problemsize % nprocs) >= myrank) ? 1:0;
	for (int i = 0 ; i < nprocs  ; ++i){
		recv[i] = i*recvsize;
	}
	return recv;
}
matrix_p  transpose(matrix_p old){
	comm_helper_p com = create_comm_list(old);
	Real * received = sendarr(com);
	matrix_p new = matrix_unsort(received);
	free(received);
	free_comm_list(com);
	return new;
}
