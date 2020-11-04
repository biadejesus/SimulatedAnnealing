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
individuo *aux;

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

double calcula_dist(int x0, int x1, int y0, int y1)
{
    double distancia;
    distancia = sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
    return distancia;
}

double distancia_total(individuo *vet_dist)
{
    double resultado = 0;
    for (int i = 0; i < max; i++)
    {
        resultado += calcula_dist(vet_dist[i].x, vet_dist[i + 1].x, vet_dist[i].y, vet_dist[i + 1].y);
    }

    return resultado;
}

void random_start()
{ //randomiza o individuo inicial
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */

    srand((time_t)ts.tv_nsec);
    int k = 0;
    int rd = 0;
    int flag = 0;

    //printf("AAAAAAAAAA %d", aux[0].id );
    // printf("\n");

    for (int i = 0; i < max; i++)
    {
        vet_ind[i].id = -1;
    }
    while (k != (max)|| flag == 0)
    {
        flag = 1;
        rd = rand() % (max);
        for (int j = 0; j < max; j++)
        {
            if (vet_ind[j].id == aux[rd].id)
            {
                flag = 0;
                
                break;
            }
        }
        if (flag != 0)
        {
            vet_ind[k] = aux[rd];
            k++;
        }
    }
}

void geraVizinho(individuo *vet_aux)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */
    srand((time_t)ts.tv_nsec);
    int n = 0;
    int m = 0;
    individuo *temp;
    temp = (individuo *)malloc(max * sizeof(individuo));
    memcpy(vet_aux, vet_ind, max* sizeof(individuo));
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
    vet_ind->fitness = distancia_total(vet_ind); //corresponde ao f(s)
    vet_aux->fitness = distancia_total(vet_aux); //corresponde ao f(si)
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

void SA()
{
    double temp = T_ini;
    int ger_atual = 0;
    int prox_ger = 0;
    int ger = 0;
    double d_ger = 1;
    int cont =0;

    individuo *copia;
    copia = (individuo *)malloc(max * sizeof(individuo));
    individuo *melhor;
    melhor = (individuo *)malloc(max * sizeof(individuo));

    for (int i = 0; i < max; i++)
    {
        vet_aux[i].x = 0;
        vet_aux[i].y = 0;
    }
    vet_ind->fitness = 0;
    vet_aux->fitness = 0;
    while (temp > T_min)
    {
            // printf("\n iteraçãotemp %f", temp);
        while (cont < 10)
        {
            cont++;
        //         printf("\n iteraçãoger %f", d_ger);
            for (int m = 0; m < 200; m++)
            {
                double s = 0;
                geraVizinho(vet_aux);     //gera um individuo igual ao atual, mas trocando um elemento de lugar
                s = delta(vet_aux, temp); //retorna 1 se f(s')<f(s) e retorna (e^delta/t) senão
                if ((s == 1.0) || (s > (double)rand() / (double)RAND_MAX))
                { //esse rand retorna um numero entre 0 e 1
                    //printf(" entrou");
                    //copia = vet_ind;
                    memcpy(copia, vet_ind, max* sizeof(individuo));

                    //troca s por s'
                    memcpy(vet_ind, vet_aux, max* sizeof(individuo));

                    //vet_aux = copia;
                    memcpy(vet_aux, copia, max* sizeof(individuo));
                    // if(vet_ind->fitness < melhor->fitness){
                    //     melhor = vet_ind;
                    //}
                    ger_atual = vet_ind->fitness;
                    prox_ger = ger;
                    ger = ger_atual;
                    d_ger = ger - prox_ger;
                }
            }
        }       
        temp *= alpha; //diminui a temperatura
    }
}

int main(int argv, char **argc)
{
    parametros(argv, argc);
    vet_aux = (individuo *)malloc(max * sizeof(individuo));
    aux = (individuo *)malloc(max * sizeof(individuo));
    memcpy(aux, vet_ind, max* sizeof(individuo));
    //random_start();
    double dis = distancia_total(vet_ind);
    printf("\n dis %f \n", dis);
    // printf("\n");
    // for (int i = 0; i < max - 1; i++)
    // {
    //     printf(" %d", vet_ind[i].id);
    // }
    printf("\n");
    SA();
    printf("\nfitness: %d", vet_ind->fitness);

    printf("\n");
}
