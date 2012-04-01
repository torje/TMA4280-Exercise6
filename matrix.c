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
	Real * sorted = malloc( sizeof(Real)*((old -> depth) )* (old-> width));
	Real * ptr = sorted;
	int stride;
	int offset = 0;
	stride = old -> depth/nprocs;
	for ( int j = 0 ; j < nprocs ; ++j){
		stride = old -> depth/nprocs;
		stride += (j <  old->depth % nprocs ) ? 1 : 0 ;
		for ( int i = 0 ; i < old -> width ; ++i ){
			memcpy( ptr , old -> vals[i]+offset,  stride*sizeof(Real));
			ptr += stride;
		}
		offset+=stride;
	}
	return sorted;
}
void matrix_print(matrix_p a);
matrix_p matrix_unsort(Real * data){
	int width = problemsize / nprocs;
	width +=  (problemsize % nprocs > myrank)? 1: 0;
	matrix_p matrix = matrix_construct( width , problemsize);
	for ( int i = 0 ; i < problemsize ; ++i){
		for ( int j = 0 ; j < width ; ++j){
			matrix -> vals[j][i] = data[i*width + j];
		}
	}
	return matrix;
}
int* create_senddispl() {
	int * sizes = malloc( nprocs* sizeof( int ));
	sizes[0] = 0;
	{
		int i;
		int block;
		int width = problemsize / nprocs;
		width +=  (problemsize %nprocs > myrank)?1: 0;
		block = problemsize / nprocs +1;
		for (i = 1 ; i < 1+(problemsize %nprocs) ; i++) {
			sizes[i] = sizes[i-1] + calc_width(i-1)*width;
		}
		block = problemsize / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = sizes[i-1]+calc_width(i-1)*width;
		}
	}
	return sizes; 
}
int calc_width(int rank){
	int width = problemsize / nprocs;
	width +=  (problemsize %nprocs > rank)?1: 0;
	return width;
}

int* c_sendcounts() {
	int * sizes = malloc( sizeof( int ) * nprocs );
	{
		int i;
		int block;
		block = problemsize / nprocs +1;
		int width = problemsize / nprocs;
		width +=  (problemsize %nprocs > myrank)?1: 0;
		for (i = 0 ; i < problemsize %nprocs ; i++) {
			sizes[i] = width*calc_width(i);
		}
		block = problemsize / nprocs;
		for (		; i < nprocs ; ++i) {
			sizes[i] = calc_width(i)*width;
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
	int width = problemsize / nprocs;
	width +=  (problemsize %nprocs > myrank)?1: 0;
	int depth = problemsize;
	int recvsize = width * depth;


	Real *recvbuf = malloc(sizeof(Real)*recvsize);
	MPI_Alltoallv( a -> data ,a -> sendcounts, a -> senddispl, MPI_DOUBLE, recvbuf, a->recvcounts, a->recvdispl , MPI_DOUBLE, MPI_COMM_WORLD);
	return recvbuf;
}

int * c_receivecounts(){
	int width = problemsize / nprocs;
	width +=  (problemsize %nprocs > myrank)?1: 0;
	int *  recv = malloc(sizeof(int)*nprocs);
	int recvsize = problemsize / nprocs;
	recvsize +=  ( ( problemsize % nprocs) > myrank) ? 1:0;
	recvsize *= width;
	for (int i = 0 ; i < nprocs ; ++i){
		recv[i] = calc_width(i)*width;
	}
	return recv;
}

int *c_recvdispl(){
	int *  recv = malloc(sizeof(int)*nprocs);
	recv[0] = 0;
	int width = problemsize / nprocs;
	width += ( problemsize % nprocs) > myrank ? 1:0;
	for (int i = 1 ; i < nprocs  ; ++i){
		recv[i] = recv[i-1] + width*calc_width(i-1);
	}
	return recv;
}

matrix_p  matrix_transpose(matrix_p old){
	comm_helper_p com = create_comm_list(old);
	Real * received = sendarr(com);
	matrix_p new = matrix_unsort(received);
	free_comm_list(com);
	return new;
}
int l_bound_(int size, int _nprocs, int rank){
	int i= 0;
	int displacement = 0;
	for ( ;i < size % _nprocs && i < rank; displacement += size/_nprocs+1, ++i);
	for ( ;i < rank; displacement += size/_nprocs, ++i);
	return displacement;
}
int h_bound_(int size, int _nprocs, int rank){
	int i= 0;
	int displacement = 0;
	for ( ;i < size % _nprocs && i <= rank; displacement += size/_nprocs+1, ++i);
	for ( ;i <= rank; displacement += size/_nprocs, ++i);
	return displacement;
}

matrix_p Gen_matrix( int size , int _nprocs, int rank, Real (*func)(int,int,Real)){
	int width = calc_width_(size, _nprocs, rank);
	int l_bound= l_bound_(size, _nprocs, rank );
	int h_bound= h_bound_(size, _nprocs, rank );
	Real scale=1/((Real)(size+1));
	matrix_p matrix = matrix_construct(h_bound - l_bound, size);
	for(int i = l_bound ; i < h_bound ; ++i ){
		for(int j = 0 ; j < size ; ++j ){
			matrix -> vals[i-l_bound][j] = func( i, j, scale);
		}
	}
	return matrix;
}

int calc_width_( int size, int _nprocs, int rank){
	int width = size / _nprocs;
	width +=  (size % _nprocs > myrank)? 1: 0;
	return width;
}

void matrix_fst( matrix_p matrix){
//#pragma omp parallel
	{
		int _depth = matrix -> depth;
		int _n = _depth+1;
		int _nn =  _n*4;
		Real *temparray= malloc(sizeof(Real)*_nn +1000000);
//#pragma omp for schedule(static)
		for ( int i = 0 ; i < matrix -> width ; ++i ){
			fst_(matrix->vals[i], &_n, temparray, &_nn);
		}
		free(temparray);
	}
}

void matrix_fst_inv( matrix_p matrix){
//#pragma omp parallel
	{
		int _depth = matrix->depth;
		int _n = _depth+1;
		int _nn = _n*4;
		Real *temparray = malloc(sizeof(Real)*_nn);
//#pragma omp for schedule(static)
		for (int j=0; j < matrix->width; j++) {
			fstinv_(matrix->vals[j], &_n, temparray, &_nn);
		}
		free(temparray);
	}
	return;
}

void matrix_strange(Real *diag, matrix_p matrix, int _nprocs, int rank) {
	int l_bound = l_bound_(matrix -> depth, _nprocs, rank),
		h_bound = h_bound_(matrix -> depth, _nprocs, rank);
	for (int j=l_bound; j <h_bound ; j++) {
		for (int i=0; i <matrix -> depth ; i++) {
			matrix -> vals[j-l_bound][i] = matrix -> vals[j-l_bound][i]/(diag[i]+diag[j]);
		}
	}
	return;
}
Real matrix_find_max(matrix_p matrix){
	Real max=0;
	for ( int i = 0 ; i < matrix -> width ; ++i){
		for ( int j = 0 ; j < matrix -> width ; ++j){
			max = (matrix->vals[i][j] > max ) ?matrix->vals[i][j] : max ;
		}
	}
	return max;
}

