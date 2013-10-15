CC1=gcc
CC2=icc
CC3=mpiicc

all:
	$(CC1) -o Sequential src/Sequential.c
	$(CC2) -o OpenMP src/OpenMP.c
	$(CC3) -o MPI src/MPI.c

clean:
	rm *.o Sequential OpenMP MPI
