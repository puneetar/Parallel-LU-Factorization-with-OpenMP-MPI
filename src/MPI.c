/*
 ============================================================================
 Name        : Assignment1.c
 Author      : Punet Arora
 Version     :
 Description : LU Factorization using OpenMP and MPI: study of scalability
 ============================================================================
 */
#include<stdio.h>
#include <stdlib.h>
//#include <omp.h>
#include <time.h>
#include <mpi.h>

double **make2dmatrix(long n);
void free2dmatrix(double ** M, long n);
void printmatrix(double **A, long n);

long matrix_size,version;
char algo;
double **A_master;


void replaceMasterMartix(double *rowk,long k,long n){
	long i=0;
	for(i=0;i<n;i++){
		A_master[k][i]=rowk[i];
	}
}

void decomposeMPI(double **A_local,long n,int argc, char *argv[])
{
	int pid;
	int partner;
	int nprocs, i,j,k,p,mymin,mymax;
	int nrows,ierr;
	int data_tag =9999;
	int root_process=0;

	MPI_Status status;

	double time_start, time_end;


	MPI_Init(&argc, &argv); /*START MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); /*DETERMINE RANK OF THIS PROCESSOR*/
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs); /*DETERMINE TOTAL NUMBER OF PROCESSORS*/
	time_start=MPI_Wtime();
	double rowk[n];
	nrows=n/nprocs;

	mymin=pid * nrows;
	mymax=mymin + nrows - 1;

	if(pid==nprocs-1 && (n-(mymax+1))>0)
		mymax=n-1;

	for(k=0;k<n;k++){
		if(k>=mymin && k<=mymax){
			for(j=k+1;j<n;j++){
				A_local[k][j]=A_local[k][j] / A_local[k][k];
			}

			if(nprocs>1){
				for(p=pid;p<nprocs;p++){
					ierr = MPI_Send( &A_local[k][0], n , MPI_DOUBLE,
							p, data_tag, MPI_COMM_WORLD);
				}
			}
		}

		if(nprocs>1){
			if(k<=mymax){
				ierr = MPI_Recv( &rowk, n, MPI_DOUBLE, MPI_ANY_SOURCE,
						data_tag, MPI_COMM_WORLD, &status);

				if(pid==nprocs-1)
					replaceMasterMartix(rowk,k,n);
			}
		}

		for(i=(((k+1) > mymin) ? (k+1) : mymin);i<=mymax;i++){
			for(j=k+1;j<n;j++){
				//A[i][j] = A[i][j] - A[i][k] * A[k][j];
				if(nprocs>1)
					A_local[i][j] = A_local[i][j] - A_local[i][k] * rowk[j];
				else
					A_local[i][j] = A_local[i][j] - A_local[i][k] * A_local[k][j];
			}
		}
	}

	time_end=MPI_Wtime();
	MPI_Finalize();  /* EXIT MPI */



	if (pid == nprocs-1){
		printf("\n*****************FINAL *****************\n\n");
		if(nprocs>1)
			printmatrix(A_master,n);
		else
			printmatrix(A_local,n);
		printf("\n*******************FINAL***************\n\n");
		printf("\n**********************************\n\n");
		printf("Algo selected :%s\n","MPI");
		printf("Size of Matrix :%lu \n",matrix_size);
		printf("Version Number : %lu\n",version);
		printf("Number of Procs : %d\n",nprocs);
		if(nprocs>1)
			printf("%s",check(A_master,n,atol(argv[2]))==1? "DECOMPOSE SUCCESSFULL\n":"DECOMPOSE FAIL\n");
		else
			printf("%s",check(A_local,n,atol(argv[2]))==1? "DECOMPOSE SUCCESSFULL\n":"DECOMPOSE FAIL\n");
		printf("DECOMPOSE TIME TAKEN : %f seconds\n",time_end-time_start);
		printf("\n**********************************\n\n");

	}
}



int checkVersion1(double **A, long n)
{
	long i, j;
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
			if(A[i][j]!=1){
				return 0;
			}
	}
	return 1;
}

void initializeVersion1(double **A, long n)
{
	long i, j;
	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			if(i<=j )
				A[i][j]=i+1;
			else
				A[i][j]=j+1;

		}
	}
}


int checkVersion2(double **A, long n)
{
	long i,j;
	for(i=0;i<n;i++){
		if(A[i][i]!=1){
			return 0;
		}
		for(j=0;j<n;j++){
			if(i!=j && A[i][j]!=2){
				return 0;
			}
		}
	}
	return 1;
}

void initializeVersion2(double **A,long n){
	long i,j, k;
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			if(i==j){
				k=i+1;
				A[i][j]=4*k-3;
			}
			else{
				A[i][j]=A[i][i]+1;
				A[j][i]=A[i][i]+1;
			}
		}
	}
}


double **getMatrix(long size,long version)
{
	double **m=make2dmatrix(size);
	switch(version){
	case 1:
		initializeVersion1(m,size);
		break;
	case 2:
		initializeVersion2(m,size);
		break;
	default:
		printf("INVALID VERSION NUMBER");
		exit(0);
	}
	return m;
}


int check(double **A, long size, long version){
	switch(version){
	case 1:
		return checkVersion1(A,size);
		break;
	case 2:
		return checkVersion2(A,size);
		break;
	default:
		printf("INVALID VERSION CHARACTER IN CHECK");
		exit(0);
	}
}

int main(int argc, char *argv[]){

	if(argc !=3){
		printf("Enter the size of matrix (N x N) where N = ");
		scanf("%lu",&matrix_size);

		printf("Enter the version number V = ");
		scanf("%lu",&version);
	}
	else{
		matrix_size=atol(argv[1]);
		version=atol(argv[2]);
	}

	double **matrix=getMatrix(matrix_size,version);

	A_master=getMatrix(matrix_size,version);

	//printmatrix(matrix,matrix_size);

	/**
	 * Code to Time the LU decompose
	 */
	clock_t begin, end;
	double time_spent;
	begin = clock();

	decomposeMPI(matrix,matrix_size,argc,argv);

	end = clock();
	time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;

	//printmatrix(matrix,matrix_size);


	//	printf("\n**********************************\n\n");
	//	printf("Algo selected :%s\n","MPI");
	//	printf("Size of Matrix :%lu \n",matrix_size);
	//	printf("Version Number : %lu\n",version);
	//	printf("%s",check(matrix,matrix_size,version)==1? "DECOMPOSE SUCCESSFULL\n":"DECOMPOSE FAIL\n");
	//	printf("DECOMPOSE TIME TAKEN : %f seconds\n",time_spent);
	//	printf("\n**********************************\n\n");

	free2dmatrix(matrix,matrix_size);
	free2dmatrix(A_master,matrix_size);
	return 0;
}


double **make2dmatrix(long n)
{
	long i;
	double **m;
	m = (double**)malloc(n*sizeof(double*));
	for (i=0;i<n;i++)
		m[i] = (double*)malloc(n*sizeof(double));
	return m;
}

// only works for dynamic arrays:
void printmatrix(double **A, long n)
{
	printf("\n *************** MATRIX ****************\n\n");
	long i, j;
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
			printf("%f ",A[i][j]);
		printf("\n");
	}
}

void free2dmatrix(double ** M, long n)
{
	long i;
	if (!M) return;
	for(i=0;i<n;i++)
		free(M[i]);
	free(M);
}
