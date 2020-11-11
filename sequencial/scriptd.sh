#! /bin/bash

echo comecando testes sequencial d

gcc sa.c -o sa.out -lm

echo sequencial compilado

./sa.out -t 1400000 -a d18512.tsp > dseq1.txt
./sa.out -t 1400000 -a d18512.tsp > dseq2.txt
./sa.out -t 1400000 -a d18512.tsp > dseq3.txt
