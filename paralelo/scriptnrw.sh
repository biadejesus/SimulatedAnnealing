#! /bin/bash

echo comecando testes nrw

mpicc sap.c -o sap.out -lm -g

echo paralelo compilado

mpirun -n 1 ./sap.out -t 1000 -a nrw1379.tsp > nrwpar1_1.txt

mpirun -n 2 ./sap.out -t 1000 -a nrw1379.tsp > nrwpar2_1.txt

mpirun -n 3 ./sap.out -t 1000 -a nrw1379.tsp > nrwpar3_1.txt

mpirun -n 4 ./sap.out -t 1000 -a nrw1379.tsp > nrwpar4_1.txt


echo acaboooo
