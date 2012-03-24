#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <omp.h>
typedef double Real;

void fun_populate_diag( Real * diag, int m , int n);
void fun_populate_b(Real **b, int m, Real h2) ;
void fun_strange(Real *diag, Real **bt, int m) ;
void fun_col_fst(Real **bt, Real *z, int m, int *n, int nn );
void fun_col_fstinv(Real **b , Real *z,  int m ,int *n , int nn);
Real fun_find_umax( Real **b ,int m );

void transpose (Real **bt, Real **b, int m);
void fst_(Real *v, int *n, Real *w, int *nn);
void fstinv_(Real *v, int *n, Real *w, int *nn);

Real *createRealArray (int n);
Real **createReal2DArray (int m, int n);
