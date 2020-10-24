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


int T_ini, pop, max, alpha, pert, ind_ini, sucess;
double coordenadas;

typedef struct{
    int x;
    int y;
    int id;
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
            T_ini = strtoul(optarg, NULL, 0); // temperatura inicial
            break;
        case 's':
            ind_ini= strtoul(optarg, NULL, 0);
            break;
        case 'm':
            max = strtoul(optarg, NULL, 0); // tamanho da população
            break;
        case 'a':
            alpha = strtoul(optarg, NULL, 0); //numero entre 0 e 1
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

int random_start(){ //gera o individuo inicial
    int aux, k=0;
    int flag = 0;
    vet_ind = (individuo *)malloc(max *sizeof(individuo));

    for(int i=0; i<max; i++){
        vet_ind[i].id = -1;
    }
    while(k!= max){
        flag = 1;
        aux = rand()% max;
        for(int j=0; j<max; j++){
            if(vet_ind[j].id == aux){
                flag=0;
                break;
            }
        }
        if(flag != 0){
            vet_ind[k].id = aux;
            k++;
        }
    }
}

void geraVizinho(individuo *vet_pai){
    int n=0, m=0, aux=0;
    for(int i =0; i< max; i++){
        vet_pai[i].id = vet_ind[i].id ;
    }
    while(n == m){
        n = rand() % (max-1);
        m = rand() % (max-1);
    }
    aux = vet_ind[n].id;
    vet_ind[n].id = vet_ind[m].id;
    vet_ind[m].id = aux;
}

double delta(individuo *vet_aux, int t){
    double find=0, faux = 0;
    double d=0;
    find = distancia_total(vet_ind);//corresponde ao f(s')
    faux = distancia_total(vet_aux);//corresponde ao f(s)
    if(faux < find){
        d = 1;
    }else{
        d = exp((find-faux)/t); // faz a parte do e^delta/t
    }
    return d;
}

void SA(){
    double temp = T_ini;
    double result = 0;
    int ger_atual =0;
    int prox_ger =0;
    int ger=0;
    int d_ger=0;
    int s=0;
    int cont = 0;
    individuo *aux;
    aux = (individuo *)malloc(sizeof(individuo *) * max);
    individuo *vet_aux;
    vet_aux = (individuo *)malloc(sizeof(individuo *) * max);
    for(int i =0; i<max; i++){
        vet_aux[i].id = 0;

    }
    while(temp > T_min){
        
        while(cont< max){// no do chines ele usa outro criterio de parada, ele usa (d_gen>0.1)
            cont++;
            
            geraVizinho(vet_aux);//gera um individuo igual ao atual, mas trocando um elemento de lugar
            s = delta(vet_aux, temp);//retorna 1 se f(s')<f(s) e retorna (e^delta/t) senão
            if((s==1)||(s>(double)rand()/RAND_MAX)){ //esse rand retorna um numero entre 0 e 1
                aux = vet_ind;
                vet_ind = vet_aux; //troca s por s'
                vet_aux = aux; 
            }
            ger_atual = distancia_total(vet_ind); 
            prox_ger = ger;
            ger = ger_atual;
            d_ger = ger - prox_ger;
            printf("\n %d \n", d_ger);
        }
        temp = alpha * temp;//diminui a temperatura
    }
}

int main(int argv, char **argc){
    parametros(argv, argc);
    srand(time(NULL));
    printf("\n%d\n", max);
    random_start();
    printf("\n");
    SA();
    for(int i=0; i< max; i++){
        printf(" %d", vet_ind[i].id);
    }
    printf("\n");
}
