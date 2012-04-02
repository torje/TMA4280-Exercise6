#include "utils.h"
#define  pi   3.14159265358979323846

void fun_populate_diag( double * diag, int m , int n){
#pragma omp parallel for
	for (int i=0; i < m; i++) {
		diag[i] = 2.*(1.-cos((i+1)*pi/(double)n));
	}
	return;
}

void fun_populate_b(double **b, int m, double h2) {
#pragma omp parallel for
	for (int j=0; j < m; j++) {
		for (int i=0; i < m; i++) {
			b[j][i] = h2;
		}
	}
	return;
}



void fun_strange(double *diag, double **bt, int m) {
#pragma omp parallel for
	for (int j=0; j < m; j++) {
		for (int i=0; i < m; i++) {
			bt[j][i] = bt[j][i]/(diag[i]+diag[j]);
		}
	}
	return;
}

void fun_col_fst(double **bt,  int m, int n, int nn ){
#pragma omp parallel
	{
		double *z = malloc(sizeof(double)*nn);
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

void fun_col_fstinv(double **b , int m ,int n , int nn){
#pragma omp parallel
	{
		double *z = malloc(sizeof(double)*nn);
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

double fun_find_umax( double **b ,int m ){
	double umax = 0.0; 
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

void transpose (double **bt, double **b, int m)
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

double *createRealArray (int n)
{
	double *a;
	int i;
	a = (double *)malloc(n*sizeof(double));
#pragma omp parallel for
	for (i=0; i < n; i++) {
		a[i] = 0.0;
	}
	return a;
}

double **createReal2DArray (int n1, int n2)
{
	int i, n;
	double **a;
	a    = (double **)malloc(n1   *sizeof(double *));
	a[0] = (double  *)malloc(n1*n2*sizeof(double));
	for (i=1; i < n1; i++) {
		a[i] = a[i-1] + n2;
	}
	n = n1*n2;
	memset(a[0],0,n*sizeof(double));
	return (a);
}
