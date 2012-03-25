#include <stdlib.h>
//stor og javlig funksjon.... prøve å bryte en ned mer. 
void MATRIX_MPI_transpose( matrix_t matrix, MPI_Comm comm, int procs, int rank) {
	int twinrank, comchan;
	real * receive_buf;

	receive_buf = malloc(sizeof(matrix_t)+sizeof(real)*(matrix -> dim (1+matrix -> dim)));
	twinrank = twinrank( matrix -> dim ,  rank);
	if ( rank == twinrank ){
		MATRIX_copy(matrix, &receive_buf);
	}
	else if (rank > twinrank ){
		comchan = twinrank;
		MPI_SendRecv();
	}else if ( rank < twinrank ){
		comchan = rank;
		MPI_RecvSend();
	}
	MPI_transpose(matrix);
	return;
}

//seriell transponering. dim*(dim-1)*0.5*3 operasjoner;
void MATRIX_transpose(matrix_t matrix){
	int i, j;

	//Dette er XOR-copy, hold tunge beint i munnen. 
	for ( i = 0 ;  i < matrix -> dim; ++i){
		for (j = i+1 ; j < matrix -> dim ; ++j ){
			REAL_xor_swap( &matrix[i][j] , &matrix[j][i] );
		}
	}
	return;
}

// XOR swap var det greieste jeg kom på i farten. Betyr antakeligvis ingenting, men den har ingen variabler å holde styr på, kun argumentene. Den brekker forresten horribelt om a og b er like, så vær så snill og ikke gi den det som argumenter. 
void REAL_xor_swap(real * a, real * b ){
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
	return;
}

//skal returnere hvilken rank prosessen skal kommunisere med. 
int twinrank(int dim, int rank){
	return ( rank % matrix -> dim) * matrix -> dim + rank / matrix -> dim;
}

matrix_t * MATRIX_dup(matrix_t *orig){
	int i;
	matrix_t * copy;
	copy = malloc(sizeof(matrix_t));
	MATRIX_infocopy(orig, copy);
	MATRIX_arraycopy(orig, copy);
	return copy;
}

void MATRIX_arraycopy(matrix_t orig, matrix_t){
	copy -> i_index = malloc (sizeof(real)*(orig -> dim (1+orig -> dim)));
	memcpy(copy -> &i_index[orig->dim], orig -> &i_index[orig -> dim ], orig -> dim * orig -> dim );
	for (i = 0 ; i < orig -> dim ; ++i){
		i_index = &i_index[orig -> dim ( 1 + orig -> dim )];
	}
	return;
}

void MATRIX_infocopy(matrix_t * orig, matrix_t * copy){
	copy -> dim = orig -> dim;
	return;
}
