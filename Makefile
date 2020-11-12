TARGETSEQ=codigos/sa.c
EXE_SEQ=sa.out
FLAGS=-lm

GCC=gcc
MPI = mpicc

TARGETPAR=codigos/sap.c
EXE_PAR=sap.out

all:sequencial paralelo

paralelo: $(TARGETPAR)
	$(MPI) $(TARGETPAR) $(FLAGS) -o $(EXE_PAR)
	mv $(EXE_PAR) codigos

sequencial: $(TARGETSEQ)
	$(GCC) $(TARGETSEQ) $(FLAGS) -o $(EXE_SEQ)
	mv $(EXE_SEQ) codigos

