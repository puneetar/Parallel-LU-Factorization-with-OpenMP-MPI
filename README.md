Problem: LU Factorization using OpenMP and MPI: study of scalability.

Description: Implement a simple LU decomposition method in C (note that you can do the implementation in many ways) that will decompose matrix A into two matrices L and U. The program needs to have the following features:

1. input matrix generation routines for A
2. LU decomposition to give you A = LU
3. output matrix verification routine
4. output the time taken to do step 2
5. input parameters should include the sizes of the matrix A and also the version of input generation (1 or 2) – description in verification section.

The goal of this assignment is to compare the performance of LU decomposition using OpenMP and MPI and the impact of increase of number of processors and the increase in size of the problem. For one problem size, keep increasing the number of processors and then keep increasing the problem size. Use large problem sizes.

Note that you are using a simple LU decomposition method and it will not work for all types of matrices. 
