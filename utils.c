#include "utils.h"
#define  pi   3.14159265358979323846

void fun_populate_diag( Real * diag, int m , int n){
#pragma omp parallel for
	for (int i=0; i < m; i++) {
		diag[i] = 2.*(1.-cos((i+1)*pi/(Real)n));
	}
	return;
}

void fun_populate_b(Real **b, int m, Real h2) {
#pragma omp parallel for
	for (int j=0; j < m; j++) {
		for (int i=0; i < m; i++) {
			b[j][i] = h2;
		}
	}
	return;
}



void fun_strange(Real *diag, Real **bt, int m) {
#pragma omp parallel for
	for (int j=0; j < m; j++) {
		for (int i=0; i < m; i++) {
			bt[j][i] = bt[j][i]/(diag[i]+diag[j]);
		}
	}
	return;
}

void fun_col_fst(Real **bt,  int m, int n, int nn ){
#pragma omp parallel
	{
		Real *z = malloc(sizeof(Real)*nn);
		int _n = n;
		int _m = m;
		int _nn = nn;
#pragma omp for schedule(static)
		for (int i=0; i < _m; i++) {
			fst_(bt[i], &_n, z, &_nn);
		}
		free(z);
	}
	return;

}

void fun_col_fstinv(Real **b , int m ,int n , int nn){
#pragma omp parallel
	{
		Real *z = malloc(sizeof(Real)*nn);
		int _n = n;
		int _m = m;
		int _nn = nn;
#pragma omp for schedule(static)
		for (int j=0; j < _m; j++) {
			fstinv_(b[j], &_n, z, &_nn);
		}
		free(z);
	}
	return;
}

Real fun_find_umax( Real **b ,int m ){
	Real umax = 0.0; 
#pragma omp parallel for
	for (size_t j=0; j < m; j++) {
		for (size_t i =0; i < m; i++) {
			if (b[j][i] > umax){
				umax = b[j][i];
			}
		}
	}
	return umax;
}

void transpose (Real **bt, Real **b, int m)
{
	int i, j;
	for (j=0; j < m; j++) {
//#pragma omp parallel for schedule(static)
		for (i=0; i < m; i++) {
			bt[j][i] = b[i][j];
		}
	}
	return;
}

Real *createRealArray (int n)
{
	Real *a;
	int i;
	a = (Real *)malloc(n*sizeof(Real));
#pragma omp parallel for
	for (i=0; i < n; i++) {
		a[i] = 0.0;
	}
	return a;
}

Real **createReal2DArray (int n1, int n2)
{
	int i, n;
	Real **a;
	a    = (Real **)malloc(n1   *sizeof(Real *));
	a[0] = (Real  *)malloc(n1*n2*sizeof(Real));
	for (i=1; i < n1; i++) {
		a[i] = a[i-1] + n2;
	}
	n = n1*n2;
	memset(a[0],0,n*sizeof(Real));
	return (a);
}
