#include <mpi.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
extern int nprocs, myrank, problemsize;
extern MPI_Comm world_com;
typedef double double;
typedef struct {
	int width,
		depth;
	double ** vals; 
} matrix_t;
typedef matrix_t * matrix_p;

void matrix_delete(matrix_p old);
typedef struct {
	int  *sendcounts;
	int  *senddispl;
	int  *recvcounts;
	int	 *recvdispl;
	double *data;
} comm_helper_t;
typedef comm_helper_t * comm_helper_p;



/*int MPI_Alltoallv(void *sendbuf, 
  int *sendcnts, 
  int *sdispls, 
  MPI_Datatype sendtype, 
  void *recvbuf, 
  int *recvcnts, 
  int *rdispls, 
  MPI_Datatype recvtype, 
  MPI_Comm comm)
  */

matrix_p matrix_construct(int width, int depth);
void matrix_delete(matrix_p old);
double * matrix_sort(matrix_p old);
void matrix_print(matrix_p a);
matrix_p matrix_unsort(double * data);
int* create_senddispl() ;
int calc_width(int rank);
int* c_sendcounts() ;
comm_helper_p create_comm_list(matrix_p data);
void free_comm_list(comm_helper_p a);
double *sendarr( comm_helper_p a);
int * c_receivecounts();
int *c_recvdispl();
matrix_p  matrix_transpose(matrix_p old);
int l_bound_(int size, int _nprocs, int rank);
int h_bound_(int size, int _nprocs, int rank);
matrix_p Gen_matrix( int size , int _nprocs, int rank, double (*func)(int,int,double));
int calc_width_( int size, int _nprocs, int rank);
void matrix_fst( matrix_p matrix);
void matrix_fst_inv( matrix_p matrix);
void matrix_strange(double *diag, matrix_p matrix, int _nprocs, int rank) ;
double matrix_find_max(matrix_p matrix);
//Lånte denne fra jabirali
void matrix_save(const char* filename, matrix_p matrix);
matrix_p subtract_matrix_func( matrix_p matrix, int size , int _nprocs, int rank, double (*func)(int,int,double));
