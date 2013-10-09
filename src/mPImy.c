#include <mpi.h>
#include <stdio.h>

void main1(int argc,char *argv[])
{
	int ierr , myid , numprocs;
	numprocs=10;
	MPI_Init(&argc ,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,& myid);

	printf( " Hello World , I am Process %d of %d \ n " , myid , numprocs);
	MPI_Finalize();




}
