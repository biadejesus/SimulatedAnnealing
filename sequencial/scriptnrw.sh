#! /bin/bash

echo comecando testes sequencial nrw

gcc sa.c -o sa.out -lm

echo sequencial compilado

./sa.out -t 1000 -a nrw1379.tsp > nrwseq1.txt
./sa.out -t 1000 -a nrw1379.tsp > nrwseq2.txt
./sa.out -t 1000 -a nrw1379.tsp > nrwseq3.txt
