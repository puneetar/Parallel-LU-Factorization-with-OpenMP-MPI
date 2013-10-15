
#!/bin/sh
module load intel/13.1
module load intel
module load intel-mpi/4.1.1
ulimit -s unlimited
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

gcc -o Sequential.out src/Sequential.c
icc -o OpenMP.out -openmp src/OpenMP.c
mpiicc -o MPI.out src/MPI.c

