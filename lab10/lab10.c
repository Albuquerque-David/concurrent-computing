/* Disciplina: Computacao Concorrente - Lab 10*/
/* Prof.: Silvana Rossetto */
/* Realizado por: David Rodrigues Albuquerque */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 10

// Variaveis globais
int s = 0; //variavel compartilhada entre as threads
sem_t mutexCons;
sem_t slotVazio;
sem_t slotCheio;  // semaforo para exclusao mutua entre as threads
sem_t mutexProd;
sem_t mutexProdFull; // Para auxiliar na exclusão mutua para preencher o buffer inteiro
sem_t mutexProdBufferVazio; // Para auxiliar na condicao de apenas inserir quando buffer vazio
int Buffer[BUFFER_SIZE];
int produtores, consumidores;

void *produtor(void *arg);
void *consumidor(void *arg);
void Insere(int item);
int Retira(void);


//funcao principal
int main(int argc, char *argv[])
{
    int t, *id;
    //inicia o semaforo binario
    sem_init(&mutexCons,0,1);
    sem_init(&mutexProd,0,1);
    sem_init(&mutexProdBufferVazio,0,1);
    sem_init(&mutexProdFull,0,1);
    sem_init(&slotCheio,0,0);
    sem_init(&slotVazio,0,BUFFER_SIZE);

    srand((unsigned int)time(NULL));

    if (argc < 3)
    {
        fprintf(stderr, "Digite: %s <numero de produtores> <numero de consumidores>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    produtores = atoi(argv[1]);
    consumidores = atoi(argv[2]);

    if (consumidores <= 0 || produtores <= 0)
    {
        fprintf(stderr, "Entre com um numero positivo de produtores/consumidores\n");
        exit(EXIT_FAILURE);
    }

    pthread_t tid_consumidoras[produtores];
    pthread_t tid_produtoras[consumidores];

    for (t = 0; t < consumidores; t++)
    {
        //aloca espaco para o identificar da thread
        if ((id = malloc(sizeof(int))) == NULL)
        {
            pthread_exit(NULL);
            return 1;
        }
        *id = t;
        //printf("--Cria a thread %d\n", t);
        if (pthread_create(&tid_consumidoras[t], NULL, consumidor, (void *)id))
        {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    for (t = 0; t < produtores; t++)
    {
        //aloca espaco para o identificar da thread
        if ((id = malloc(sizeof(int))) == NULL)
        {
            pthread_exit(NULL);
            return 1;
        }
        *id = t;
        //printf("--Cria a thread %d\n", t);
        if (pthread_create(&tid_produtoras[t], NULL, produtor, (void *)id))
        {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    //--espera todas as threads terminarem
    for (t = 0; t < consumidores; t++)
    {
        if (pthread_join(tid_consumidoras[t], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }
    for (t = 0; t < produtores; t++)
    {
        if (pthread_join(tid_produtoras[t], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }
    printf("Valor de s = %d\n", s);

    sem_destroy(&mutexCons);
    sem_destroy(&mutexProd);
    sem_destroy(&slotCheio);
    sem_destroy(&slotVazio);

    pthread_exit(NULL);
}

void *produtor(void *arg) {
    int elemento;
    long int id = (long int) arg;
    while(1) {
        
        sem_wait(&mutexProdBufferVazio);
        sem_wait(&mutexProdFull);
        printf("O produtor %ld ira inserir.\n", id);
        for(int i = 0; i < BUFFER_SIZE; i++) {
            elemento = rand() % 100;
            Insere(elemento);
        }
        sem_post(&mutexProdFull);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int elemento;
    long int id = (long int) arg;
    while(1) {
        printf("O consumidor %ld ira retirar.\n", id);
        elemento = Retira();
    }
    pthread_exit(NULL);
}

void Insere(int item) {
    static int in=0;

    sem_wait(&slotVazio);
    sem_wait(&mutexProd);

    Buffer[in] = item;
    printf("Produtor inserindo elemento %d na posicao %d.\n", item, in);
    in = (in+1) % BUFFER_SIZE;
     
    sem_post(&mutexProd);
    sem_post(&slotCheio);
}

int Retira (void) {
    int item;
    static int out = 0;
    
    sem_wait(&slotCheio);
    sem_wait(&mutexCons);

    item = Buffer[out];
    printf("Consumidor retirando elemento %d na posicao %d.\n", item, out);
    out = (out + 1) % BUFFER_SIZE;
    if(out == 0) {
        sem_post(&mutexProdBufferVazio);
    }
    sem_post(&mutexCons);
    sem_post(&slotVazio);

    return item;
}