#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <omp.h>

void fun_populate_diag( double * diag, int m , int n);
void fun_populate_b(double **b, int m, double h2) ;
void fun_strange(double *diag, double **bt, int m) ;
void fun_col_fst(double **bt, int m, int n, int nn );
void fun_col_fstinv(double **b , int m ,int n , int nn);
double fun_find_umax( double **b ,int m );

void transpose (double **bt, double **b, int m);
void fst_(double *v, int *n, double *w, int *nn);
void fstinv_(double *v, int *n, double *w, int *nn);

double *createRealArray (int n);
double **createReal2DArray (int m, int n);
