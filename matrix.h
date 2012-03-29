#include <mpi.h>
#include <stdio.h>
extern int nprocs, myrank, problemsize;
extern MPI_Comm world_com;
typedef double Real;
typedef struct {
	int width,
		depth;
	Real ** vals; 
} matrix_t;
typedef matrix_t * matrix_p;

void matrix_delete(matrix_p old);
typedef struct {
	int  *sendcounts;
	int  *senddispl;
	int  *recvcounts;
	int	 *recvdispl;
	Real *data;
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
Real *matrix_sort(matrix_p old);
matrix_p matrix_unsort(Real * data);
int *create_senddispl() ;
int *c_sendcounts() ;
comm_helper_p create_comm_list(matrix_p data);
void free_comm_list(comm_helper_p a);
Real *sendarr( comm_helper_p a);
int *c_receivecounts();
int *c_recvdispl();
matrix_p  transpose(matrix_p old);
