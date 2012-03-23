#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
typedef double Real;

void fun_populate_diag( Real * diag, int m , int n);
void fun_populate_b(Real **b, int m, Real h2) ;
void fun_strange(int m, Real *diag, Real **bt) ;
void fun_col_fst_( int m , Real *z, int * n , int nn,  Real ** bt);
void fun_col_fstinv_( int m , Real *z, int * n , int nn, Real ** b);
Real fun_find_umax(int m , Real **b);

void transpose (Real **bt, Real **b, int m);
void fst_(Real *v, int *n, Real *w, int *nn);
void fstinv_(Real *v, int *n, Real *w, int *nn);

Real *createRealArray (int n);
Real **createReal2DArray (int m, int n);
