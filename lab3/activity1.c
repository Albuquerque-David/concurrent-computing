#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

float *arrayOfElements;
long long int numberOfElements;
float sLim, iLim;
int nthreads;

typedef struct
{
    int id;
    int start, end;
} tArgs;

float *initializeArray(int randomNumber);
int *calculateOcurrencesSequentially();
void *calculateOcurrencesConcurrently(void *arg);
int checkMatrixMultiplication();
void printMatrixs();

int main(int argc, char *argv[])
{
    pthread_t *tid;
    tArgs *args;
    double start, end, delta;
    int seqOcurrences, concOcurrences;
    float randomNum;

    srand((unsigned int)time(NULL));
    randomNum = ((float)rand()/(float)(RAND_MAX) * 25.0);

    if (argc < 4)
    {
        printf("Digite: %s <numero de threads> <numero de elementos do vetor> <limite inferior> <limite superior>\n", argv[0]);
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
        fprintf(stderr,"Entre com um numero positivo de dimensão da matriz\n");
        return 1;
    }

    if (nthreads > numberOfElements)
    {
        nthreads = numberOfElements;
    }

    // Inicializacao do array
    initializeArray(randomNum);

    // Calculo de ocorrencias do elemento no array realizada de forma sequencial
    GET_TIME(start);

    seqOcurrences = calculateOcurrencesSequentially();

    GET_TIME(end);
    delta = end - start;
    printf("O calculo sequencial de ocorrencias levou: %lf ms\n", delta);

    // Calculo da quantidade de aparicoes do elemento no array realizada de forma concorrente
    GET_TIME(start);

    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL)
    {
        puts("Erro ao alocar tid\n");
        return 2;
    }

    args = (tArgs *)malloc(sizeof(tArgs) * nthreads);
    if (args == NULL)
    {
        puts("Erro ao alocar args\n");
        return 2;
    }

    for(int thread = 0; thread < nthreads; thread++) {
        
        (args + thread)->start = thread * ((dim) / nthreads); // De onde a thread iniciará o calculo, alocando dinamicamente de acordo com a quantidade de threads e tamanho do array
        (args + thread)->end = (thread * ((dim) / nthreads)) + ((dim) / nthreads); // Onde a thread terminará o calculo, alocando dinamicamente de acordo com a quantidade de threads e tamanho do array
        (args + thread)->id = thread;
        
        if (pthread_create(tid + thread, NULL, multiplyMatrixsConcurrently, (void*) (args + thread))) {
            printf("Erro ao criar threads para a funcao multiplyMatrixsConcurrently\n");
            exit(-1);
        } else {
            //printf("Thread criada com sucesso. Tid: %ld\n",*(tid + thread));
        }

    }

    // Aguarda as threads finalizarem
    for (int thread = 0; thread < nthreads; thread++) {
        if (pthread_join(*(tid + thread), NULL)) {
            printf("Erro ao aguardar as threads terminarem a execução\n"); 
            exit(-1); 
        } else {
            //printf("Thread finalizada com sucesso. Tid: %ld\n", *(tid + thread));
        }
    }

    GET_TIME(end);
    delta = end - start;
    printf("O calculo concorrente de ocorrencias levou: %lf ms\n", delta);

    // checando resultado

    GET_TIME(start);

    int isWrong = checkMatrixMultiplication();
    if(isWrong){
        printf("Erro no calculo da matriz\n");
    } else {
        printf("O resultado do calculo esta correto\n");
    }

    GET_TIME(end);
    delta = end - start;
    printf("A checagem do resultado levou: %lf ms\n", delta);
    
    //liberacao de memoria
    GET_TIME(start);

    free(arrayOfElements);
    free(args);
    free(tid);

    GET_TIME(end);
    delta = end - start;
    printf("A liberacao de memoria levou: %lf ms\n", delta);

    return 0;
}

float *initializeArray(int randomNumber) {
    arrayOfElements = (float *) malloc(sizeof(float) * numberOfElements);
    if (arrayOfElements == NULL) {
        fprintf(stderr,"Erro ao alocar array\n"); 
        return 1;
    }

    for(int i = 0; i < numberOfElements; i++) {
        arrayOfElements[i] = randomNumber;
    }

    return 0;
}

int *calculateOcurrencesSequentially() {

    int ocurrencesLocal = 0;

    for(int i = 0; i < numberOfElements; i++) {
        if (iLim < arrayOfElements[i] < sLim) {
            ocurrencesLocal;
        }
    }

    return ocurrencesLocal;
}

void *calculateOcurrencesConcurrently(void *arg)
{
    int start, end;
    tArgs *args = (tArgs *) arg;
    int *localOcurrences;
    start = args->start;
    end = args->end;

    *localOcurrences = 0;
    
    for(int i = start; i < end; i++) {
        if (iLim < arrayOfElements[i] < sLim) {
            *localOcurrences++;
        }
            
    }

    pthread_exit((void *) localOcurrences);
}

int checkMatrixMultiplication(){

    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            for(int k = 0; k < dim; k++) {
                if(matSequential[i * dim + j] != matConcurrent[i * dim + j])
                    return 1;
            }   
        }
    }

    return 0;
}

void printMatrixs() {
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++) {
            printf("%.1f ", mat1[i * dim + j]);
        }
        printf("\n");
    }

    printf("--------\n\n");

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++) {
            printf("%.1f ", mat2[i * dim + j]);
        }
        printf("\n");
    }

    printf("--------\n\n");


    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++) {
            printf("%.1f ", matSequential[i * dim + j]);
        }
        printf("\n");
    }

    printf("--------\n\n");

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++) {
            printf("%.1f ", matConcurrent[i * dim + j]);
        }
        printf("\n");
    }

    printf("--------\n\n");

}