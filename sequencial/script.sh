#! /bin/bash

echo comecando testes sequencial

gcc sa.c -o sa.o -lm

echo sequencial compilado

time ./sa.o -t 2000 -s d15112.txt > seq.txt