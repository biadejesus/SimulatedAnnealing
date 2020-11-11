#! /bin/bash

echo comecando testes sequencial pla

gcc sa.c -o sa.out -lm

echo sequencial compilado

./sa.out -t 1400000 -a pla7397.tsp > plaseq1.txt
./sa.out -t 1400000 -a pla7397.tsp > plaseq2.txt
./sa.out -t 1400000 -a pla7397.tsp > plaseq3.txt
