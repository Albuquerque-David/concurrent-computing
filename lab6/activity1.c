/* Grade: Concurrent Computing - 2021.2 */
/* Prof.: Silvana Rossetto */
/* Made by.: David R Albuquerque - 120047390 */
/* Lab: 6 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Variaveis globais */
int sLim, iLim;
int bloqueadas = 0;
int *vetor;
int nthreads;

void barreira(int nthreads);
void *tarefa(void *arg);

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Funcao principal */
int main(int argc, char *argv[])
{
    
    srand((unsigned int)time(NULL));
    iLim = 0;
    sLim = 10;
    int randomNum;

    if (argc < 2)
    {
        fprintf(stderr, "Digite: %s <numero de threads> \n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);
    vetor = (int *) malloc(sizeof(int) * nthreads);

    pthread_t threads[nthreads];
    int id[nthreads];
    int *returnValue;
    int vetorResult[nthreads];

    /* Inicializa o vetor com valores aleatorios */
    for (int i = 0; i < nthreads; i++) {
        randomNum = rand() % sLim;
        vetor[i] = randomNum;
    }

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    /* Cria as threads */
    for (int i = 0; i < nthreads; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, tarefa, (void *)&id[i]);
    }

    /* Espera todas as threads completarem */
    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(threads[i], (void**) &returnValue);
        vetorResult[i] = *returnValue;
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

    int somaResult = vetorResult[0];
    int isWrong = 0;

    for(int i = 0; i < nthreads; i++) {
        if(vetorResult[i] != somaResult) {
            isWrong = 1;
        }
    }

    if(isWrong) {
        printf("O resultado da operacao esta incorreta.\n");
    } else {
        printf("O resultado da operacao esta correto.\n");
    }

    free(returnValue);

    return 0;
}

// Funcao barreira
void barreira(int nthreads)
{
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads - 1))
    {
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&x_cond);
        bloqueadas = 0;
    }
    else
    {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

// Funcao das threads
void *tarefa(void *arg)
{
    int id = *(int *)arg;
    int *soma = 0;
    soma = (int*) malloc(sizeof(int));
    *soma = 0;

    for (int passo = 0; passo < nthreads; passo++)
    {        
        for (int i = 0; i < nthreads; i++) {
            *soma += vetor[i];
        }

        // Sincronizacao condicional
        printf("Thread %d terminou a soma %d. Iteracao: %d.\n", id, *soma, passo);
        barreira(nthreads);
        pthread_mutex_lock(&x_mutex);
        int randomNum = rand() % sLim;
        vetor[id] = randomNum;
        // Preenche com novo valor aleatório
        pthread_mutex_unlock(&x_mutex);
        barreira(nthreads);
    }
    pthread_exit((void *) soma);
}