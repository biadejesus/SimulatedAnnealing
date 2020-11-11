#! /bin/bash

echo comecando testes d

mpicc sap.c -o sap.out -lm -g

echo paralelo compilado

mpirun -n 1 ./sap.out -t 1400000 -a d18512.tsp > dpar1_1.txt
mpirun -n 1 ./sap.out -t 1400000 -a d18512.tsp > dpar1_2.txt
mpirun -n 1 ./sap.out -t 1400000 -a d18512.tsp > dpar1_3.txt
mpirun -n 2 ./sap.out -t 1400000 -a d18512.tsp > dpar2_1.txt
mpirun -n 2 ./sap.out -t 1400000 -a d18512.tsp > dpar2_2.txt
mpirun -n 2 ./sap.out -t 1400000 -a d18512.tsp > dpar2_3.txt
mpirun -n 3 ./sap.out -t 1400000 -a d18512.tsp > dpar3_1.txt
mpirun -n 3 ./sap.out -t 1400000 -a d18512.tsp > dpar3_2.txt
mpirun -n 3 ./sap.out -t 1400000 -a d18512.tsp > dpar3_3.txt
mpirun -n 4 ./sap.out -t 1000000 -a d18512.tsp > dpar4_1.txt
mpirun -n 4 ./sap.out -t 1400000 -a d18512.tsp > dpar4_2.txt
mpirun -n 4 ./sap.out -t 1400000 -a d18512.tsp > dpar4_3.txt

echo acaboooo
