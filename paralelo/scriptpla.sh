#! /bin/bash

echo comecando testes pla

mpicc sap.c -o sap.out -lm -g

echo paralelo compilado

mpirun -n 1 ./sap.out -t 1000 -a pla7397.tsp > plapar1_1.txt
mpirun -n 2 ./sap.out -t 1000 -a pla7397.tsp > plapar2_1.txt
mpirun -n 3 ./sap.out -t 1000 -a pla7397.tsp > plapar3_1.txt
mpirun -n 4 ./sap.out -t 1000 -a pla7397.tsp > plapar4_1.txt

echo acaboooo
