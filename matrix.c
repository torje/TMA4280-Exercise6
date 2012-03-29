#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include <unistd.h>

matrix_p matrix_construct(int width, int depth){
	matrix_p matrix = malloc(sizeof(matrix_t));
	matrix -> vals = malloc(sizeof(Real)* width);
	matrix -> width = width;
	matrix -> depth =  depth;
	for ( int i = 0 ;  i < width ; ++i){
		matrix -> vals [i] = malloc(sizeof(Real)*depth);
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
	int * aaa = calloc( 1024, sizeof( int ) );
	return aaa;
	//printf("	sizes[0] = 0;\n");
	//sizes[0] = 0;
	//{
	//	int i;
	//	int block;
	//	printf("	block = problemsize / nprocs +1;\n");
	//	block = problemsize / nprocs +1;
	//	printf("	for (i = 1 ; i < problemsize %nprocs ; i++) {\n");
	//	for (i = 1 ; i < problemsize %nprocs ; i++) {
	//		sizes[i] = sizes[i-1]+block;
	//	}
	//	block = problemsize / nprocs;
	//	printf("	for (		; i < nprocs ; ++i) {\n");
	//	for (		; i < nprocs ; ++i) {
	//		sizes[i] = sizes[i-1]+block;
	//	}
	//}
	//return sizes; 
}

int* c_sendcounts() {
	int * sizes = malloc( sizeof( int ) * nprocs );
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
	printf("	comm_helper_p comms = malloc(sizeof(comm_helper_t));\n");
	comm_helper_p comms = malloc(sizeof(comm_helper_t));
	printf("	comms -> sendcounts = c_sendcounts();\n");
	comms -> sendcounts = c_sendcounts();
	printf("	comms -> data= matrix_sort(data);\n");
	comms -> data= matrix_sort(data);
	//printf("	comms -> senddispl= create_senddispl();\n");
	//comms -> senddispl= create_senddispl();
	printf("	comms -> recvdispl = c_recvdispl();\n");
	comms -> recvdispl = c_recvdispl();
	printf("	comms -> recvcounts = c_receivecounts();\n");
	comms -> recvcounts = c_receivecounts();
	printf("	return comms;\n");
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
	MPI_Alltoallv( a -> data ,a -> senddispl, a -> senddispl, MPI_DOUBLE, recvbuf, a->recvcounts, a->recvdispl , MPI_DOUBLE, MPI_COMM_WORLD);
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
	printf("comm_helper_p com = create_comm_list(old); \n");
	comm_helper_p com = create_comm_list(old);
	printf("Real * received = sendarr(com); \n");
	Real * received = sendarr(com);
	printf("matrix_p new = matrix_unsort(received); \n");
	matrix_p new = matrix_unsort(received);
	//printf("free(received);\n");
	//free(received);
	printf("free_comm_list(com); \n");
	free_comm_list(com);
	printf("return \n");
	return new;
}
