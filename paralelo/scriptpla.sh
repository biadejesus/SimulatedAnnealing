#! /bin/bash

echo comecando testes pla

mpicc sap.c -o sap.out -lm -g

echo paralelo compilado

mpirun -n 1 ./sap.out -t 1400000 -a pla7397.tsp > plapar1_1.txt
mpirun -n 1 ./sap.out -t 1400000 -a pla7397.tsp > plapar1_2.txt
mpirun -n 1 ./sap.out -t 1400000 -a pla7397.tsp > plapar1_3.txt
mpirun -n 2 ./sap.out -t 1400000 -a pla7397.tsp > plapar2_1.txt
mpirun -n 2 ./sap.out -t 1400000 -a pla7397.tsp > plapar2_2.txt
mpirun -n 2 ./sap.out -t 1400000 -a pla7397.tsp > plapar2_3.txt
mpirun -n 3 ./sap.out -t 1400000 -a pla7397.tsp > plapar3_1.txt
mpirun -n 3 ./sap.out -t 1400000 -a pla7397.tsp > plapar3_2.txt
mpirun -n 3 ./sap.out -t 1400000 -a pla7397.tsp > plapar3_3.txt
mpirun -n 4 ./sap.out -t 1000000 -a pla7397.tsp > plapar4_1.txt
mpirun -n 4 ./sap.out -t 1400000 -a pla7397.tsp > plapar4_2.txt
mpirun -n 4 ./sap.out -t 1400000 -a pla7397.tsp > plapar4_3.txt

echo acaboooo
