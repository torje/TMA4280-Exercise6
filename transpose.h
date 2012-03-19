#include <stdlib.h>
typedef double real;
struct matrix_st;
	int dim;
	real ** i_index;
	real * j_index;
};
typedef struct matrix_st matrix_t;
void MATRIX_MPI_transpose( matrix_t matrix, MPI_Comm comm, int procs, int rank);
void MPI_transpose(matrix_t matrix);
void REAL_xor_swap(real * a, real * b );
int twinrank(int dim, int rank);
matrix_t * MATRIX_dup(matrix_t *orig);
void MATRIX_arraycopy(matrix_t orig, matrix_t);
void MATRIX_infocopy(matrix_t * orig, matrix_t * copy);
