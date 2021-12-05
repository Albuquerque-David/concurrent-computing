#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

float *arrayOfElements;
long long int numberOfElements;
float sLim, iLim;
int nthreads;

int initializeArray();
int calculateOcurrencesSequentially();
void *calculateOcurrencesConcurrently(void *arg);
int checkOcurrencesCalc(int seqOcurrences, int concOcurrences);
void printArraysAndOcurrences(int seqOcurrences, int concOcurrences);

int main(int argc, char *argv[])
{
    pthread_t *tid;
    double start, end, delta;
    int seqOcurrences, concOcurrences;
    int *returnValue;

    srand((unsigned int)time(NULL));

    if (argc < 5)
    {
        fprintf(stderr, "Digite: %s <numero de threads> <numero de elementos do vetor> <limite inferior> <limite superior>\n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);
    numberOfElements = atoll(argv[2]);
    iLim = atof(argv[3]);
    sLim = atof(argv[4]);

    if (nthreads <= 0)
    {
        fprintf(stderr,"Entre com um numero positivo de threads\n");
        return 1;
    }

    if (numberOfElements <= 0)
    {
        fprintf(stderr, "Entre com um numero positivo de numero de elementos do array\n");
        return 1;
    }

    if (iLim > sLim)
    {
        int aux = iLim;
        iLim = sLim;
        sLim = aux;
    }

    if (nthreads > numberOfElements)
    {
        nthreads = numberOfElements;
    }

    // Inicializacao do array
    initializeArray();

    // Calculo de ocorrencias do elemento no array realizada de forma sequencial
    GET_TIME(start);

    seqOcurrences = calculateOcurrencesSequentially();

    GET_TIME(end);
    delta = end - start;
    printf("O calculo sequencial de ocorrencias levou: %lf ms\n", delta);

    // Calculo da quantidade de aparicoes do elemento no array realizada de forma concorrente
    GET_TIME(start);

    concOcurrences = 0;

    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL)
    {
        fprintf(stderr, "Erro ao alocar tid\n");
        return 2;
    }

    for(long int thread = 0; thread < nthreads; thread++) {
        
        if (pthread_create(tid + thread, NULL, calculateOcurrencesConcurrently, (void*) ( thread ))) {
            fprintf(stderr, "Erro ao criar threads para a funcao calculateOcurrencesConcurrently\n");
            return -1;
        } else {
            //printf("Thread criada com sucesso. Tid: %ld\n",*(tid + thread));
        }

    }

    // Aguarda as threads finalizarem
    for (int thread = 0; thread < nthreads; thread++) {
        if (pthread_join(*(tid + thread), (void**) &returnValue)) {
            fprintf(stderr, "Erro ao aguardar as threads terminarem a execução\n"); 
            return -1;
        }
        concOcurrences += *returnValue;
    }

    GET_TIME(end);
    delta = end - start;
    printf("O calculo concorrente de ocorrencias levou: %lf ms\n", delta);

    // checando resultado

    GET_TIME(start);

    int isWrong = checkOcurrencesCalc(seqOcurrences, concOcurrences);
    if(isWrong){
        printf("Erro no calculo de ocorrencias\n");
    } else {
        printf("O resultado do calculo esta correto\n");
    }

    GET_TIME(end);
    delta = end - start;
    printf("A checagem do resultado levou: %lf ms\n", delta);
    //printArraysAndOcurrences(seqOcurrences, concOcurrences);
    
    //liberacao de memoria
    GET_TIME(start);

    free(arrayOfElements);
    free(tid);

    GET_TIME(end);
    delta = end - start;
    printf("A liberacao de memoria levou: %lf ms\n", delta);


    return 0;
}

int initializeArray() {
    float randomNum;
    arrayOfElements = (float *) malloc(sizeof(float) * numberOfElements);
    if (arrayOfElements == NULL) {
        fprintf(stderr,"Erro ao alocar array\n"); 
        return 1;
    }

    for(int i = 0; i < numberOfElements; i++) {
        randomNum = ((float)rand()/(float)(RAND_MAX) * (sLim * 2));
        arrayOfElements[i] = randomNum;
    }

    return 0;
}

int calculateOcurrencesSequentially() {

    int localOcurrences = 0;

    for(int i = 0; i < numberOfElements; i++) {
        if (iLim < arrayOfElements[i] && arrayOfElements[i] < sLim) {
            localOcurrences++;
        }
    }

    return localOcurrences;
}

void *calculateOcurrencesConcurrently(void *arg)
{
    long long int start, end;
    long int id = (long int) arg;
    int *localOcurrences;
    start = id * ((numberOfElements) / nthreads);
    end = (id * ((numberOfElements) / nthreads)) + ((numberOfElements) / nthreads);

    localOcurrences = (int*) malloc(sizeof(int));
    if(localOcurrences==NULL) {
        exit(1);
    }
    *localOcurrences = 0;
    
    for(int i = start; i < end; i++) {
        if (iLim < arrayOfElements[i] && arrayOfElements[i] < sLim) {
            *localOcurrences += 1;
        }
    }

    pthread_exit((void *) localOcurrences);
}

int checkOcurrencesCalc(int seqOcurrences, int concOcurrences){

    if(seqOcurrences != concOcurrences) {
        return 1;
    }   

    return 0;
}

void printArraysAndOcurrences(int seqOcurrences, int concOcurrences) {
    for(int i = 0; i < numberOfElements; i++) {
        printf("[%.2f] ", arrayOfElements[i]);
    }

    printf("\n");

    printf("Ocorrencias sequencial: %d\n", seqOcurrences);
    printf("Ocorrencias concorrente: %d\n", concOcurrences);
}