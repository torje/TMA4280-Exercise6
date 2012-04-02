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
#include "utils.h"


int main(int argc, char **argv )
{
	double *diag, **b, **bt;
	double pi, h, umax;
	int i, j, n, m, nn;

	/* the total number of grid points in each spatial direction is (n+1) */
	/* the total number of degrees-of-freedom in each spatial direction is (n-1) */
	/* this version requires n to be a power of 2 */

	if( argc < 2 ) {
		printf("need a problem size\n");
		return 1;
	}

	n  = atoi(argv[1]);
	m  = n-1;
	nn = 4*n;

	diag = createRealArray (m);
	b    = createReal2DArray (m,m);
	bt   = createReal2DArray (m,m);

	h    = 1./(double)n;
	pi   = 4.*atan(1.);

	printf("fun_populate_diag(diag, m , n);");
	printf("\n");
	fun_populate_diag(diag, m , n);
	printf(" fun_populate_b(b, m, h*h); ");
	printf("\n");
	fun_populate_b(b, m, h*h);
	printf("fun_col_fst(b, m, n, nn); ");
	printf("\n");
	fun_col_fst(b, m, n, nn);


	printf("transpose (bt,b,m); ");
	printf("\n");
	transpose (bt,b,m);

	printf("fun_col_fstinv(  bt , m , n , nn ); ");
	printf("\n");
	fun_col_fstinv(  bt , m , n , nn );


	printf("fun_strange(diag, bt, m); ");
	printf("\n");
	fun_strange(diag, bt, m);

	printf("fun_col_fst(bt, m, n, nn);");
	printf("\n");
	fun_col_fst(bt, m, n, nn);

	printf(" transpose (b,bt,m); ");
	printf("\n");
	transpose (b,bt,m);

	printf(" fun_col_fstinv( b,m, n, nn); ");
	printf("\n");
	fun_col_fstinv( b, m, n, nn);

	printf(" umax = fun_find_umax(b, m); ");
	printf("\n");
	umax = fun_find_umax(b, m);

	printf (" umax = %e \n",umax);
}
