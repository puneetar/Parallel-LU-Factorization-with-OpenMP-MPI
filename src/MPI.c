///*
// ============================================================================
// Name        : Assignment1.c
// Author      : Punet Arora
// Version     :
// Description : LU Factorization using OpenMP and MPI: study of scalability
// ============================================================================
// */
//#include<stdio.h>
//
//double **make2dmatrix(long n);
//void free2dmatrix(double ** M, long n);
//void printmatrix(double **A, long n);
//
//long matrix_size,version;
//
//void decompose(double **A, long n)
//{
//	long i,j,k;
//	for(k=0;k<n;k++){
//		for(j=k+1;j<n;j++)
//			A[k][j]=A[k][j]/A[k][k];
//
//		for(i=k+1;i<n;i++)
//			for(j=k+1;j<n;j++)
//				A[i][j]=A[i][j] - A[i][k] * A[k][j];
//	}
//}
//
//double **make2dmatrix(long n)
//{
//	long i;
//	double **m;
//	m = (double**)malloc(n*sizeof(double*));
//	for (i=0;i<n;i++)
//		m[i] = (double*)malloc(n*sizeof(double));
//	return m;
//}
//
//void initializeVersion1(double **A, long n)
//{
//	long i, j;
//	for (i=0;i<n;i++){
//		for (j=0;j<n;j++){
//			if(i<=j )
//				A[i][j]=i+1;
//			else
//				A[i][j]=j+1;
//
//		}
//	}
//}
//
//void free2dmatrix(double ** M, long n)
//{
//	long i;
//	if (!M) return;
//	for(i=0;i<n;i++)
//		free(M[i]);
//	free(M);
//}
//
//// only works for dynamic arrays:
//void printmatrix(double **A, long n)
//{
//	long i, j;
//	for (i=0;i<n;i++)
//	{
//		for (j=0;j<n;j++)
//			printf("%f ",A[i][j]);
//		printf("\n");
//	}
//}
//
//double **get2dmatrix(long size,long version)
//{
//	double **m=make2dmatrix(size);
//	switch(version){
//	case 1:
//		initializeVersion1(m,size);
//		break;
//	case 2:
//		break;
//	default:
//		printf("INVALID VERSION NUMBER");
//		break;
//	}
//	return m;
//}
//
//int main(int argc, char *argv[]){
//
//	if(argc !=3){
//		printf("Enter the size of matrix (N x N) where N = ");
//		scanf("%lu",&matrix_size);
//
//		printf("Enter the version number V = ");
//		scanf("%lu",&version);
//	}
//	else{
//		char data[] = "1 1000\n";
//		matrix_size=strtol(argv[1], argv[1], 10);
//		version=	strtol(data, argv[2], 10);
//	}
//
//	//	matrix_size=5;
//	//	version=1;
//
//	printf("Size of Matrix :%lu \n",matrix_size);
//	printf("Version Number : %lu\n\n",version);
//
//	double **matrix=get2dmatrix(matrix_size,version);
//
//	printf("\n ***************ORIGINAL MATRIX ****************\n\n");
//	printmatrix(matrix,matrix_size);
//
//	decompose(matrix,matrix_size);
//
//	printf("\n ***************AFTER DECOMPOSE ****************\n\n");
//	printmatrix(matrix,matrix_size);
//
//	free2dmatrix(matrix,matrix_size);
//
//	printf("\n ***************LAST DROP ****************\n\n");
//
//
//	return 0;
//}
