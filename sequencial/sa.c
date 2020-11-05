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
#include <float.h>
#define T_min 0.00001
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

int *melhorCaminho;

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
            melhorCaminho = (int *)malloc(max * sizeof(int));
            int i = 0;
            int x;
            int y;
            int id;
            for (int i = 0; i < max; i++)
            {
                fscanf(arq, "%d %d %d", &id, &x, &y);
                melhorCaminho[i] = id;
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
    double valor;
    valor = sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
    return valor;
}

double distancia_total(individuo *vet_dist, int *caminho)
{
    double resultado = 0;
    for (int i = 0; i < max - 1; i++)
    {
        resultado += calcula_dist(vet_dist[caminho[i]].x, vet_dist[caminho[i + 1]].x, vet_dist[caminho[i]].y, vet_dist[caminho[i + 1]].y);
    }

    return resultado + calcula_dist(vet_dist[0].x, vet_dist[max - 1].x, vet_dist[0].y, vet_dist[max - 1].y);
}

// void *random_start()
// { //randomiza o individuo inicial
//     struct timespec ts;
//     clock_gettime(CLOCK_MONOTONIC, &ts);

//     /* using nano-seconds instead of seconds */

//     srand((time_t)ts.tv_nsec);
//     int k = 0;
//     int rd = 0;
//     int flag = 0;

//     //printf("AAAAAAAAAA %d", aux[0].id );
//     // printf("\n");

//     for (int i = 0; i < max; i++)
//     {
//         vet_ind[i].id = -1;
//     }
//     while (k != (max) || flag == 0)
//     {
//         flag = 1;
//         rd = rand() % (max);
//         for (int j = 0; j < max; j++)
//         {
//             if (vet_ind[j].id == aux[rd].id)
//             {
//                 flag = 0;

//                 break;
//             }
//         }
//         if (flag != 0)
//         {
//             vet_ind[k] = aux[rd];
//             k++;
//         }
//     }
// }

void *geraVizinho(int *caminhoAtual, int *proxCaminho)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    srand((time_t)ts.tv_nsec);
    int aux = 0;
    for (int i = 0; i < max; i++)
    {
        proxCaminho[i] = caminhoAtual[i];
    }

    int n = (rand() % (max - 1)) + 1;
    int m = (rand() % (max - 1)) + 1;

    aux = proxCaminho[n];
    proxCaminho[n] = proxCaminho[m];
    proxCaminho[m] = aux;
}

int *vizinhoProximo()
{
    int *caminho;
    int *visitado;
    double menorDist;
    int maisProx;
    int comeco;
    double distancia = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */

    srand((time_t)ts.tv_nsec);

    caminho = (int *)malloc(max * sizeof(int));
    visitado = (int *)malloc(max * sizeof(int));
    for (int i = 0; i < max; i++)
    {
        visitado[i] = 0;
    }
    comeco = rand() % max;
    visitado[comeco] = 1;
    caminho[0] = comeco;

    for (int i = 1; i < max; i++)
    {
        menorDist = DBL_MAX;

        maisProx = 0;
        for (int j = 0; j < max; j++)
        {
            distancia = calcula_dist(vet_ind[caminho[i - 1]].x, vet_ind[j].x, vet_ind[caminho[i - 1]].y, vet_ind[j].y);
            if (!visitado[j] && distancia < menorDist)
            {
                menorDist = distancia;
                maisProx = j;
            }
        }
        caminho[i] = maisProx;
        visitado[maisProx] = 1;
    }

    free(visitado);
    return caminho;
}

void SA()
{
    double temp = T_ini;
    int ger_atual = 0;
    int prox_ger = 0;
    int ger = 0;
    double d_ger = 1;
    int cont = 0;
    double dist = 0;
    double delta = 0;
    int *caminhoAtual;
    int *proxCaminho;
    caminhoAtual = vizinhoProximo();
    proxCaminho = (int *)malloc(max * sizeof(int));
    dist = distancia_total(vet_ind, caminhoAtual);

    while (cont < 10)
    {

        cont++;
        //int cont = 0;
        // printf("\n iteraçãotemp %f", temp);
        // while (cont < 1)
        // {

        //         printf("\n iteraçãoger %f", d_ger);

        geraVizinho(caminhoAtual, proxCaminho);               //gera um individuo igual ao atual, mas trocando um elemento de lugar
        delta = distancia_total(vet_ind, proxCaminho) - dist; //retorna 1 se f(s')<f(s) e retorna (e^delta/t) senão
        //printf(" %f", delta);
        if (((delta < 0) || (dist > 0)) && ((exp((-delta) / temp)) > (double)rand() / RAND_MAX))
        { //esse rand retorna um numero entre 0 e 1

            for (int i = 0; i < max; i++)
            {
                caminhoAtual[i] = proxCaminho[i];
            }
            if (distancia_total(vet_ind, melhorCaminho) > distancia_total(vet_ind, caminhoAtual))
            {
                for (int i = 0; i < max; i++)
                {
                    melhorCaminho[i] = caminhoAtual[i];
                }
            }
            dist += delta;
        }
        //     cont++;
        // }

        temp *= alpha; //diminui a temperatura
    }
    printf("\n%f", distancia_total(vet_ind, caminhoAtual));
    printf("\nmelhor %f", distancia_total(vet_ind, melhorCaminho));

    // if (distancia_total(vet_ind, melhorCaminho) > distancia_total(vet_ind, caminhoAtual))
    // {
    //     for (int i = 0; i < max; i++)
    //     {
    //         melhorCaminho[i] = caminhoAtual[i];
    //     }
    // }
}

int main(int argv, char **argc)
{
    parametros(argv, argc);
    //random_start();
    // printf("\n");
    // for (int i = 0; i < max - 1; i++)
    // {
    //     printf(" %d", vet_ind[i].id);
    // }
    printf("\n");
    SA();
    printf("\nfitness: %f", distancia_total(vet_ind, melhorCaminho));

    printf("\n");
}
