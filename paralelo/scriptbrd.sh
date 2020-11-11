#! /bin/bash

echo comecando testes brd

mpicc sap.c -o sap.out -lm -g

echo paralelo compilado

mpirun -n 1 ./sap.out -t 1400000 -a brd14051.tsp > brdpar1_1.txt
mpirun -n 1 ./sap.out -t 1400000 -a brd14051.tsp > brdpar1_2.txt
mpirun -n 1 ./sap.out -t 1400000 -a brd14051.tsp > brdpar1_3.txt
mpirun -n 2 ./sap.out -t 1400000 -a brd14051.tsp > brdpar2_1.txt
mpirun -n 2 ./sap.out -t 1400000 -a brd14051.tsp > brdpar2_2.txt
mpirun -n 2 ./sap.out -t 1400000 -a brd14051.tsp > brdpar2_3.txt
mpirun -n 3 ./sap.out -t 1400000 -a brd14051.tsp > brdpar3_1.txt
mpirun -n 3 ./sap.out -t 1400000 -a brd14051.tsp > brdpar3_2.txt
mpirun -n 3 ./sap.out -t 1400000 -a brd14051.tsp > brdpar3_3.txt
mpirun -n 4 ./sap.out -t 1000000 -a brd14051.tsp > brdpar4_1.txt
mpirun -n 4 ./sap.out -t 1400000 -a brd14051.tsp > brdpar4_2.txt
mpirun -n 4 ./sap.out -t 1400000 -a brd14051.tsp > brdpar4_3.txt

echo acaboooo
