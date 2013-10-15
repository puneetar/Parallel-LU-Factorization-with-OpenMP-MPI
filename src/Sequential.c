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
double **A_master;

void decomposeSerial(double **A, long n)
{
	printf("DECOMPOSE SEQUENTIAL CALLED");
	long i,j,k;
	for(k=0;k<n;k++){
		for(j=k+1;j<n;j++)
			A[k][j]=A[k][j]/A[k][k];

		for(i=k+1;i<n;i++)
			for(j=k+1;j<n;j++)
				A[i][j]=A[i][j] - A[i][k] * A[k][j];
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

	//printmatrix(matrix,matrix_size);

	/**
	 * Code to Time the LU decompose
	 */
	clock_t begin, end;
	double time_spent;
	begin = clock();

	decomposeSerial(matrix,matrix_size);

	end = clock();
	time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;

	//printmatrix(matrix,matrix_size);

	printf("\n**********************************\n\n");
	printf("Algo selected :%s\n","Sequential");
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
