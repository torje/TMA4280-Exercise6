#include <mpi.h>
typedef double Real;
typedef struct {
	int width,
		depth;
	Real ** vals; 
} matrix_t;
typedef matrix_t * matrix_p;
matrix_p matrix_construct(int width, int depth);
void matrix_delete(matrix_p old);
typedef struct {
	int nprocs;
	int problemsize;
	int *partitions;
	int *range;
	int *senddispl;
	Real *data;
} comm_helper_t;
typedef comm_helper_t * comm_helper_p;

int* processlist(int nprocs);
comm_helper_p create_comm_list(int nprocs, int problemsize, matrix_p data);
void free_comm_list(comm_helper_p a);

int* create_senddispl( int size , int nprocs);

/*int MPI_Alltoallv(void *sendbuf, 
		int *sendcnts, 
		int *sdispls, 
        MPI_Datatype sendtype, 
		void *recvbuf, 
		int *recvcnts, 
        int *rdispls, 
		MPI_Datatype recvtype, 
		aMPI_Comm comm)
		*/
extern int nprocs, myrank, problemsize;
