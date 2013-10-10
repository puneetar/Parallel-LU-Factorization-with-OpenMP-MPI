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

double **make2dmatrix(long n);
void free2dmatrix(double ** M, long n);
void printmatrix(double **A, long n);

long matrix_size,version;
char algo;

void decomposeSerial(double **A, long n)
{
	long i,j,k;
	for(k=0;k<n;k++){
		for(j=k+1;j<n;j++)
			A[k][j]=A[k][j]/A[k][k];

		for(i=k+1;i<n;i++)
			for(j=k+1;j<n;j++)
				A[i][j]=A[i][j] - A[i][k] * A[k][j];
	}
}

void decomposeOpenMP(double **A, long n)
{
/*	begin
	int i, j, k, pid;  pid = process id in [0,nprocs-1], assigned
	int rows, mymin, mymax;  indices of first and last rows assigned
	rows <- n/nprocs;  assume it divides for simplicity
	mymin <- pid * rows; mymax <- mymin + rows - 1;
	for k <- 0 to n-1 do
		if (mymin <= k <= mymax)  if pivot row is among those assigned to me
			for j <- k+1 to n-1 do  i scales row k
				A[k,j] <- A[k,j] / A[k,k];
		endif
		BARRIER(bar1, nprocs);  everybody else waits
		for i <- max(k+1, mymin) to mymax do  a block of rows from me
			for j <- k+1 to n-1 do  update a row
				A[i,j] <- A[i,j] - A[i,k] * A[k,j];
		endfor
	endfor*/

#pragma
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



double **getMatrix(long size,long version)
{
	double **m=make2dmatrix(size);
	switch(version){
	case 1:
		initializeVersion1(m,size);
		break;
	case 2:

	default:
		printf("INVALID VERSION NUMBER");
		break;
	}
	return m;
}

void decompose(double **A, long size, char algo){
	switch(algo){
	case 's':
		decomposeSerial(A,size);
		break;
	case 'o':

	case 'm':

	default:
		printf("INVALID ALGO CHARACTER");
		break;
	}
}

int check(double **A, long size, long version){
	switch(version){
	case 1:
		return checkVersion1(A,size);
		break;
	case 2:

	default:
		printf("INVALID ALGO CHARACTER");
		return -1;
		break;
	}
}

int main(int argc, char *argv[]){

	if(argc !=4){
		printf("Enter the size of matrix (N x N) where N = ");
		scanf("%lu",&matrix_size);

		printf("Enter the version number V = ");
		scanf("%lu",&version);

		printf("Enter the algo to run Squential(s) OR OpenMP(o) OR MPI(m) = ");
		scanf("%c",&algo);
	}
	else{
		//		char data[] = "1 1000\n";
		//		matrix_size=strtol(argv[1], argv[1], 10);
		//		version=	strtol(data, argv[2], 10);
		algo=(char)*argv[1];
		matrix_size=atol(argv[2]);
		version=atol(argv[3]);
	}

	//	matrix_size=5;
	//	version=1;

	double **matrix=getMatrix(matrix_size,version);

	printmatrix(matrix,matrix_size);

	/**
	 * Code to Time the LU decompose
	 */
	clock_t begin, end;
	double time_spent;
	begin = clock();
		decompose(matrix,matrix_size,algo);
	end = clock();
	time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;

	printmatrix(matrix,matrix_size);

	printf("\n**********************************\n\n");
	printf("Algo selected :%c\n",algo);
	printf("Size of Matrix :%lu \n",matrix_size);
	printf("Version Number : %lu\n",version);
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
