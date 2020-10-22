#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/time.h>
#define T_min 0.000001


int temp_ini, pop, max, alpha, pert, ind_ini, sucess;
double coordenadas;
int *final;

typedef struct{
    int x;
    int y;
}individuo;

individuo *vet_ind;
void parametros(int argv, char **args)
{
    int opt;
    while ((opt = getopt(argv, args, "t:s:m:a:p:l:h")) != -1)
    {
        switch (opt)
        {
        case 't':
            temp_ini = strtoul(optarg, NULL, 0); 
            break;
        case 's':
             ind_ini= strtoul(optarg, NULL, 0);
            break;
        case 'm':
            max = strtoul(optarg, NULL, 0);
            break;
        case 'a':
            alpha = strtoul(optarg, NULL, 0);
            break;
        case 'p':
            pop = strtoul(optarg, NULL, 0);
            break;
        case 'l':
            sucess = strtoul(optarg, NULL, 0);
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

int calcula_dist(individuo vet1, individuo vet2){
    int distancia;
    distancia = sqrt(pow((vet2.x-vet1.x),2) + pow((vet2.y-vet1.y),2));
    return distancia;
}

int distancia_total(individuo *vet_dist){
    double resultado = 0;
    for(int i=0; i< max-1; i++){
        resultado += calcula_dist(vet_dist[i], vet_dist[i+1]);
    }
    return resultado;
}

int random_start(){
    int aux, k=0;
    int flag = 0;
    final = (int *)malloc(max *sizeof(int));

    for(int i=0; i<max; i++){
        final[i] = -1;
    }
    while(k!= max){
        flag = 1;
        aux = rand()% max;
        for(int j=0; j<max; j++){
            if(final[j] == aux){
                flag=0;
                break;
            }
        }
        if(flag != 0){
            final[k] = aux;
            k++;
        }
    }
}

// void SA(temp_ini, pop, max, pert, alpha,ind_ini, sucess){
//     int s = ind_ini;
//     int temp = temp_ini;
//     int itera = 0;
//     int aux;
//     int delta;
//     vet_ind = (int *)malloc(sizeof(int *) * max);
//     while(temp > T_min){
//         while(itera< max){
//             itera++;
//             srand(time(NULL));
//             aux = rand() % (max);
//             // delta = distancia(vet_ind[aux], vet_ind[s]);
//             // if(delta < 0){

//             // }
//         }
//     }

// }

int main(int argv, char **argc){
    parametros(argv, argc);
    printf("\n%d\n", max);
    random_start();
    for(int i=0; i< max; i++){
        printf(" %d", final[i]);
    }
    printf("\n");
}
