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

Real habitant(int a, int b, Real scale){
	return 5*M_PI*M_PI*scale*scale*sin(7*M_PI*(b)*scale)*sin(7*M_PI*(a)*scale);
}

void matrix_print(matrix_p a){
	for ( int i =0; i <a->width; ++i ){
		for ( int j = 0 ; j < a->depth; ++j){
			printf(" %lf " , a->vals[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv )
{
	MPI_Init( &argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);

	Real *diag, **b, **bt;
	Real pi, h, umax;
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

	h    = 1./(Real)n;
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
	//umax = matrix_find_max(matrix1);

	//printf (" umax = %e \n",umax);
	printf("%.2lf\n", time);
	MPI_Finalize();

}
