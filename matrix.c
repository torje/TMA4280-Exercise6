#include "matrix.h"

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
	Real * sorted = malloc( sizeof(Real)*((old -> depth) )* (old-> width) );
	Real * ptr = sorted;
	int stride;
	int offset = 0;
	stride = old -> depth/nprocs;
	for ( int j = 0 ; j < old -> depth /nprocs ; ++j){
		stride = old -> depth/nprocs;
		stride += (j <  old->depth % nprocs ) ? 1 : 0 ;
		for ( int i = 0 ; i < old -> width ; ++i ){
			printf("element %lf\n",old -> vals[i][0]+offset);
			memcpy( ptr , old -> vals[i]+offset,  stride*sizeof(Real));
			ptr += stride;
		}
		offset+=stride;
	}
	printf("\n");
	return sorted;
}
void matrix_print(matrix_p a);
matrix_p matrix_unsort(Real * data){
	int width = problemsize / nprocs;
	width +=  (problemsize % nprocs > myrank)? 1: 0;
	printf("vidde = %d\n",  width);
	printf("\n");
	printf("matrix_p matrix = matrix_construct( width , problemsize);\n");
	matrix_p matrix = matrix_construct( width , problemsize);
	printf("for ( int i = 0 ; i < width ; ++i){\n");
	for ( int i = 0 ; i < width ; ++i){
		for ( int j = 0 ; j < problemsize ; ++j){
			printf("i = %d , j = %d\n", i, j);
			matrix -> vals[i][j] = data[i*problemsize + j];
			printf("%lf\n", data[i*width + j]);
		}
	}
	matrix_print(matrix);
	free(data);
	return matrix;
}
int* create_senddispl() {
	int * sizes = malloc( 1024* sizeof( int ) );
	printf("	sizes[0] = 0;\n");
	sizes[0] = 0;
	{
		int i;
		int block;
		block = problemsize / nprocs +1;
		printf("block = %d\n", block);
		for (i = 1 ; i < 1+(problemsize %nprocs) ; i++) {
			sizes[i] = sizes[i-1]+block*problemsize;
		}
		block = problemsize / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = sizes[i-1]+block*problemsize;
		}
	}
	return sizes; 
}

int* c_sendcounts() {
	int * sizes = malloc( sizeof( int ) * nprocs );
	{
		int i;
		int block;
		block = problemsize / nprocs +1;
		printf("block %d\n" , block);
		for (i = 0 ; i < problemsize %nprocs ; i++) {
			sizes[i] = block*problemsize;
		}
		block = problemsize / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = block*problemsize;
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
	for ( int i = 0; i < nprocs ; ++i){
		printf("sendcnts = %d,  senddispls = %d, receivecounts =%d, receivedispls = %d\n",  a -> sendcounts[i], a -> senddispl[i],  a->recvcounts[i], a->recvdispl[i]); 
	}
	MPI_Alltoallv( a -> data ,a -> sendcounts, a -> senddispl, MPI_DOUBLE, recvbuf, a->recvcounts, a->recvdispl , MPI_DOUBLE, MPI_COMM_WORLD);
	return recvbuf;
}

int * c_receivecounts(){
	int *  recv = malloc(sizeof(int)*nprocs);
	int recvsize = problemsize / nprocs;
	recvsize +=  ( ( problemsize % nprocs) > myrank) ? 1:0;
	recvsize *= problemsize;
	for (int *i = recv ; i < recv + nprocs ; ++i){
		*i = recvsize;
	}
	return recv;
}

int *c_recvdispl(){
	int *  recv = malloc(sizeof(int)*nprocs);
	recv[0] = 0;
	int recvsize = problemsize / nprocs;
	recvsize += ( problemsize % nprocs) > myrank ? 1:0;
	recvsize *= problemsize;
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
