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
#include <omp.h>
#include <time.h>
//#include <mpi.h>

double **make2dmatrix(long n);
void free2dmatrix(double ** M, long n);
void printmatrix(double **A, long n);

long matrix_size,version;
char algo;


void decomposeOpenMP(double **A, long n)
{
	printf("\nDECOMPOSE OPENMP CALLED\n");

	long i,j,k,rows,mymin,mymax;
	int pid=0;
	int nprocs;

#pragma omp parallel shared(A,n,nprocs) private(i,j,k,pid,rows,mymin,mymax)
	{
#ifdef _OPENMP
		nprocs=omp_get_num_threads();
#endif

#ifdef _OPENMP
		pid=omp_get_thread_num();
#endif

	//	printf("1. I am proc no %d out of %d\n",pid,nprocs);

		rows=n/nprocs;
		mymin=pid * rows;
		mymax=mymin + rows - 1;

		if(pid==nprocs-1 && (n-(mymax+1))>0)
			mymax=n-1;

		for(k=0;k<n;k++){
			if(k>=mymin && k<=mymax){
				//#pragma omp for schedule(static)
				for(j=k+1;j<n;j++){
					A[k][j] = A[k][j]/A[k][k];
				}
			}

#pragma omp barrier
			for(i=(((k+1) > mymin) ? (k+1) : mymin);i<=mymax;i++){
				//#pragma omp for schedule(static)
				for(j=k+1;j<n;j++){
					A[i][j] = A[i][j] - A[i][k] * A[k][j];
				}
			}
		}
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

	if(argc !=4){
		printf("Enter the size of matrix (N x N) where N = ");
		scanf("%lu",&matrix_size);

		printf("Enter the version number V = ");
		scanf("%lu",&version);
	}
	else{
		matrix_size=atol(argv[1]);
		version=atol(argv[2]);
	}
	long num_threads=atol(argv[3]);
	if(num_threads<1){
		num_threads=5;
	}
	omp_set_num_threads(num_threads);

	double **matrix=getMatrix(matrix_size,version);

	//printmatrix(matrix,matrix_size);

	/**
	 * Code to Time the LU decompose
	 */
	clock_t begin, end;
	double time_spent;
	begin = clock();

	decomposeOpenMP(matrix,matrix_size);

	end = clock();
	time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;

	//printmatrix(matrix,matrix_size);


	printf("\n**********************************\n\n");
	printf("Algo selected :%s\n","OpenMP");
	printf("Size of Matrix :%lu \n",matrix_size);
	printf("Version Number : %lu\n",version);
	printf("Number of Procs : %lu\n",num_threads);
	printf("%s",check(matrix,matrix_size,version)==1? "DECOMPOSE SUCCESSFULL\n":"DECOMPOSE FAIL\n");
	printf("DECOMPOSE TIME TAKEN : %f seconds\n",time_spent);
	printf("\n**********************************\n\n");

	free2dmatrix(matrix,matrix_size);
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
