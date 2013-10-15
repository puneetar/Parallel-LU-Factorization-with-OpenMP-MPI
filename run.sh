
#!/bin/bash
clear
clear
cd src/
#
#  Compile the program with GCC.
#
echo "Compiling.."
#gcc -fopenmp -o OpenMP OpenMP.c 
#gcc -o Sequential Sequential.c 
mpicc -o MPI MPI.c 
echo "Compiling Complete"

#./Sequential s 1000 2
#./Sequential s 1000 1

export OMP_NUM_THREADS=1
echo "Run with $OMP_NUM_THREADS thread."
#./OpenMP 1000 2 6
#./Sequential 1000 2
mpirun -np $OMP_NUM_THREADS MPI 10 1



#  Discard the executable file.
rm Sequential
rm MPI
rm OpenMP
#
echo "********************COMPLETED********************"

