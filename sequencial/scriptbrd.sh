#! /bin/bash

echo comecando testes sequencial brd

gcc sa.c -o sa.out -lm

echo sequencial compilado

./sa.out -t 1000 -a brd14051.tsp > brdseq1.txt
./sa.out -t 1000 -a brd14051.tsp > brdseq2.txt
./sa.out -t 1000 -a brd14051.tsp > brdseq3.txt
