#! /bin/bash

echo comecando testes paralelo

mpicc sap.c -o sap.out -lm

echo paralelo compilado

time mpirun -n 1 ./sap.out -t 2000 -s d15112.txt > par1.txt
time mpirun -n 2 ./sap.out -t 2000 -s d15112.txt > par2.txt
time mpirun -n 3 ./sap.out -t 2000 -s d15112.txt > par3.txt
time mpirun -n 4 ./sap.out -t 2000 -s d15112.txt > par4.txt

echo acaboooo