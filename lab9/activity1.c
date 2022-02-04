/* Grade: Concurrent Computing - 2021.2 */
/* Prof.: Silvana Rossetto */
/* Made by.: David R Albuquerque */
/* Lab: 9 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS 5

/* Variaveis globais */
int x = 0;
sem_t em_init, em_end; // condicionais
sem_t em; //exclusao mutua

/* Thread A */
void *A(void *arg)
{
    printf("Seja bem-vindo!\n");
    sem_post(&em_init);
    sem_post(&em_init);
    sem_post(&em_init);
    pthread_exit(NULL);
}

/* Thread B */
void *B(void *arg)
{
    /* Recebe como param uma string qualquer para ser mostrada. */
    char *string = (char *)arg;

    sem_wait(&em_init);
    printf("%s", string);
    sem_wait(&em); //entrada na secao critica
    x++;
    if(x == 3) {
        sem_post(&em_end);
    }
    sem_post(&em); //saida da secao critica
    string = NULL;
    free(string);
    pthread_exit(NULL);
}

/* Thread C */
void *C(void *arg)
{
    sem_wait(&em_end);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[])
{
    int i;
    pthread_t threads[NTHREADS];

    sem_init(&em, 0, 1);
    sem_init(&em_init, 0, 0);
    sem_init(&em_end, 0, 0);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, C, NULL);
    pthread_create(&threads[1], NULL, B, (void *)"Fique a vontade.\n");
    pthread_create(&threads[2], NULL, B, (void *)"Sente-se por favor.\n");
    pthread_create(&threads[3], NULL, B, (void *)"Aceita um copo d'agua?\n");
    pthread_create(&threads[4], NULL, A, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
