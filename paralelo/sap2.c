#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>
#define T_min 0.000001
#define alpha 0.9999

int max;
double T_ini;
double coordenadas;
FILE *arq;

typedef struct
{
    int x;
    int y;
    int id;
    int fitness;
} individuo;

individuo *vet_ind;
individuo *vet_aux;

void parametros(int argv, char **args)
{
    int opt;
    while ((opt = getopt(argv, args, "t:s:m:h")) != -1)
    {
        switch (opt)
        {
        case 't':
            T_ini = strtoul(optarg, NULL, 0); // temperatura inicial
            break;
        case 's':
            arq = fopen(optarg, "r");
            if (arq == NULL)
            {
                printf("\nerro ao abrir arquivo\n");
                exit(EXIT_FAILURE);
            }
            char lixo[50000];
            fgets(lixo, 200, arq);
            fgets(lixo, 200, arq);
            fgets(lixo, 200, arq);
            fscanf(arq, "\nDIMENSION : %d\n", &max);
            fgets(lixo, 200, arq);
            fgets(lixo, 200, arq);
            vet_ind = (individuo *)malloc(max * sizeof(individuo));
            int i = 0;
            int x;
            int y;
            int id;
            for (int i = 0; i < max; i++)
            {
                //printf("\nentrou\n");
                fscanf(arq, "%d %d %d", &id, &x, &y);
                vet_ind[i].id = id;
                vet_ind[i].x = x;
                vet_ind[i].y = y;
                // printf("\n %d %d, %d", vet_ind[i].id, vet_ind[i].x, vet_ind[i].y);
            }
            fclose(arq);
            break;
        case 'm':
            max = strtoul(optarg, NULL, 0); // tamanho da população
            break;
        case 'h':
            printf("\n---Ajuda---\n");
            printf("\nPara executar o programa utlize a flag -t para definir o numero de threads (pivos) que deseja (EX: -t 8)");
            printf("\nCaso queira gerar numeros aleatorios como entrada utilize a flag -n para definir o tamanho do vetor de entrada (EX: -n 300)");
            printf("\nCaso queira inserir um arquivo como entrada utilize a flag -a para inserir um arquivo (EX: -a entrada.txt)\n");
            printf("\nAo final do código existem 'printf's comentados, estes mostram o vetor original e o ordenado. Caso queira verificar a saída descomente-os. Caso queira redirecionar a saida para um arquivo, descomente-os e insira ' >> saida.txt' ao final da linha de comando da execucao.\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            abort();
        }
    }
}
int calcula_dist(int x0, int x1, int y0, int y1)
{
    int distancia;
    distancia = sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
    return distancia;
}

int distancia_total(individuo *vet_dist)
{
    double resultado = 0;
    for (int i = 0; i < max - 1; i++)
    {
        resultado += calcula_dist(vet_dist[i].x, vet_dist[i + 1].x, vet_dist[i].y, vet_dist[i + 1].y);
    }
    //printf(" %f \n", resultado);
    return resultado;
}

int random_start()
{ //randomiza o individuo inicial
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */
    srand((time_t)ts.tv_nsec);
    //srand(1);
    int aux, k = 0;
    int flag = 0;

    for (int i = 0; i < max; i++)
    {
        vet_ind[i].id = -1;
    }
    while (k != max)
    {
        flag = 1;
        aux = rand() % max;
        for (int j = 0; j < max; j++)
        {
            if (vet_ind[j].id == aux)
            {
                flag = 0;
                break;
            }
        }
        if (flag != 0)
        {
            vet_ind[k].id = aux;
            k++;
        }
    }
    printf("\n");
    for (int i = 0; i < max; i++)
    {
        printf(" %d", vet_ind[i].id);
    }
    printf("\n");
}

void geraVizinho(individuo *vet_aux)
{
    int n = 0, m = 0;
    individuo *temp;
    temp = (individuo *)malloc(max * sizeof(individuo));
    for (int i = 0; i < max; i++)
    {
        vet_aux[i] = vet_ind[i];
    }
    while (n == m)
    {
        n = rand() % (max - 1);
        m = rand() % (max - 1);
    }
    temp[0] = vet_aux[n];
    vet_aux[n] = vet_aux[m];
    vet_aux[m] = temp[0];
}

double delta(individuo *vet_aux, int t)
{
    double find = 0, faux = 0;
    double d = 0;
    vet_ind->fitness = distancia_total(vet_ind); //corresponde ao f(s')
    vet_aux->fitness = distancia_total(vet_aux); //corresponde ao f(s)
    if (vet_aux->fitness < vet_ind->fitness)
    {
        d = 1.0;
    }
    else
    {
        d = exp((vet_ind->fitness - vet_aux->fitness) / t); // faz a parte do e^delta/t
    }
    return d;
}

int main(int argv, char **argc)
{
    parametros(argv, argc);

    MPI_Init(NULL, NULL);
    vet_aux = (individuo *)malloc(max * sizeof(individuo));
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    random_start();

    // double temp = T_ini;
    // double result = 0;
    // int ger_atual = 0;
    // int prox_ger = 0;
    // int ger = 0;
    // int d_ger = 1;
    // float s = 0.0;
    // individuo *aux;
    // aux = (individuo *)malloc(max * sizeof(individuo));
    // for (int i = 0; i < max; i++)
    // {
    //     vet_aux[i].x = 0;
    //     vet_aux[i].y = 0;
    // }
    // vet_aux->fitness = 0;
    // MPI_Barrier(MPI_COMM_WORLD);
    // while (temp > T_min)
    // {
    //     printf("a");
    //     while (d_ger > 0.1)
    //     {
    //         printf("b");
    //         for (int m = 0; m < 20000 / world_size; m++)
    //         {
    //             geraVizinho(vet_aux);     //gera um individuo igual ao atual, mas trocando um elemento de lugar
    //             s = delta(vet_aux, temp); //retorna 1 se f(s')<f(s) e retorna (e^delta/t) senão
    //             if ((s == 1.0) || (s > (double)rand() / RAND_MAX))
    //             { //esse rand retorna um numero entre 0 e 1
    //                 aux = vet_ind;

    //                 //troca s por s'
    //                 vet_ind = vet_aux;

    //                 vet_aux = aux;
    //             }
    //         }
    //         ger_atual = vet_ind->fitness;
    //         if (world_rank == 0)
    //             prox_ger = ger;
    //         MPI_Reduce(&ger_atual, &ger, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    //         if (world_rank == 0)
    //             d_ger = ger - prox_ger;
    //         MPI_Bcast(&d_ger, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //         //d_ger = ger - prox_ger;
    //     }
    //     temp *= alpha; //diminui a temperatura
    // }
    // MPI_Reduce(&ger_atual, &ger, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    // if (world_rank == 0)
    // {
    //     printf("\nfitness: %d", ger);
    //     printf("\n");
    // }

    MPI_Finalize();
}