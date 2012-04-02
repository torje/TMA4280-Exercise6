/*
   C-program to solve the two-dimensional Poisson equation on 
   a unit square using one-dimensional eigenvalue decompositions
   and fast sine transforms

   einar m. ronquist
   ntnu, october 2000
   revised, october 2001
   */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#include "utils.h"
#include "matrix.h"
int nprocs, problemsize, width, myrank, namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;

double habitant(int a, int b, double scale){
	return 5*M_PI*M_PI*scale*scale*sin(2*M_PI*(b+1)*scale)*sin(M_PI*(a+1)*scale);
}

double deduct(int a, int b, double scale){
	return sin(2*M_PI*(b+1)*scale)*sin(M_PI*(a+1)*scale);
}

int main(int argc, char **argv )
{
	MPI_Init( &argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);

	double *diag, **b, **bt;
	double pi, h, umax;
	double time;
	int i, j, n, m, nn;

	/* the total number of grid points in each spatial direction is (n+1) */
	/* the total number of degrees-of-freedom in each spatial direction is (n-1) */
	/* this version requires n to be a power of 2 */

	if( argc < 2 ) {
		printf("need a problem size\n");
		MPI_Finalize();
		return 1;
	}

	n  = atoi(argv[1]);
	m  = n-1;
	nn = 4*n;
	problemsize=m;
	diag = createRealArray (m);
	b    = createReal2DArray (m,m);
	matrix_p matrix1 = Gen_matrix( m, nprocs, myrank, &habitant);
	bt   = createReal2DArray (m,m);

	h    = 1./(double)n;
	pi   = 4.*atan(1.);

	fun_populate_diag(diag, m , n);

	time = MPI_Wtime();
	matrix_fst( matrix1);
	
	matrix_p matrix2 = matrix_transpose(matrix1);
	matrix_fst_inv(matrix2);
	matrix_strange(diag, matrix2, nprocs, myrank);
	matrix_fst(matrix2);
	matrix1 = matrix_transpose(matrix2);
	matrix_fst_inv(matrix1);
	time -= MPI_Wtime();
	time = -time;
	matrix_save("out.dat", matrix1);
	subtract_matrix_func( matrix1, m, nprocs, myrank, &deduct);
	umax = matrix_find_max(matrix1);

	printf ("%e, ",umax);
	MPI_Finalize();


}
