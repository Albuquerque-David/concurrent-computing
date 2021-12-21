/* Grade: Concurrent Computing - 2021.2 */
/* Prof.: Silvana Rossetto */
/* Made by.: David R Albuquerque */
/* Lab: 5 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5
#define EXECUTE_START_THREADS 0
#define EXECUTE_MIDDLE_THREADS 1
#define EXECUTE_END_THREADS 2

/* Variaveis globais */
int execution_control = 0;
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *A(void *arg)
{
    pthread_mutex_lock(&x_mutex);
    /* Executa o trecho apenas quando as threads iniciais forem permitidas. */
    while (!(execution_control == EXECUTE_START_THREADS)) {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    execution_control = EXECUTE_MIDDLE_THREADS;
    printf("Seja bem-vindo!\n");
    /* Dispara um aviso para que as threads intermediarias possam ser executadas. */
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread B */
void *B(void *arg)
{
    /* Recebe como param uma string qualquer para ser mostrada. */
    char *string = (char*) arg;

    pthread_mutex_lock(&x_mutex);
    /* Executa o trecho apenas quando as threads intermediarias forem permitidas. */
    while (!(execution_control == EXECUTE_MIDDLE_THREADS)) {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    x++;
    printf("%s", string);
    if(x == NTHREADS - 2) {
        execution_control = EXECUTE_END_THREADS;
        /* Dispara um aviso para que as threads finais possam ser executadas. */
        pthread_cond_broadcast(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    string = NULL;
    free(string);
    pthread_exit(NULL);
}

/* Thread C */
void *C(void *arg)
{
    pthread_mutex_lock(&x_mutex);
    /* Executa o trecho apenas quando as threads finais forem permitidas. */
    while (!(execution_control == EXECUTE_END_THREADS)) {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    x++;
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[])
{
    int i;
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, C, NULL);
    pthread_create(&threads[1], NULL, B, (void*) "Fique a vontade.\n");
    pthread_create(&threads[2], NULL, B, (void*) "Sente-se por favor.\n");
    pthread_create(&threads[3], NULL, B, (void*) "Aceita um copo d'agua?\n");
    pthread_create(&threads[4], NULL, A, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina a execucao */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
}
