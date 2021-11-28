#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "./libs/timer.h"

float *mat1;
float *mat2;
float *matSequential;
float *matConcurrent;
int nthreads;
int dim;

typedef struct
{
    int id;
    int start, end;
} tArgs;

float *initializeMatrix(int randomNumber);
int *multiplyMatrixsSequentially();
void *multiplyMatrixsConcurrently(void *arg);
int checkMatrixMultiplication();
void printMatrixs();

int main(int argc, char *argv[])
{
    pthread_t *tid;
    tArgs *args;
    double start, end, delta;

    if (argc < 3)
    {
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    if (nthreads <= 0)
    {
        printf("Entre com um numero positivo de threads\n");
        exit(1);
    }

    if (dim <= 0)
    {
        printf("Entre com um numero positivo de dimensão da matriz\n");
        exit(1);
    }

    if (nthreads > dim)
    {
        nthreads = dim;
    }

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

    // Inicializacao das matrizes
    initializeMatrix(2);
    

    // Multiplicacao de matrizes realizada de forma sequencial
    GET_TIME(start);

    multiplyMatrixsSequentially(dim);

    GET_TIME(end);
    delta = end - start;
    printf("A multiplicacao sequencial levou: %lf ms\n", delta);

    GET_TIME(start);

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
    printf("A multiplicacao concorrente levou: %lf ms\n", delta);

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

    free(mat1);
    free(mat2);
    free(matSequential);
    free(matConcurrent);
    free(args);
    free(tid);

    GET_TIME(end);
    delta = end - start;
    printf("A liberacao de memoria levou: %lf ms\n", delta);

    return 0;
}

float *initializeMatrix(int randomNumber) {
    mat1 = (float *) malloc(sizeof(float) * dim * dim);
    if (mat1 == NULL) {
        printf("Erro ao alocar matriz\n"); 
        exit(2);
    }

    mat2 = (float *) malloc(sizeof(float) * dim * dim);
    if (mat2 == NULL) {
        printf("Erro ao alocar matriz\n"); 
        exit(2);
    }

    matSequential = (float *) malloc(sizeof(float) * dim * dim);
    if (matSequential == NULL) {
        printf("Erro ao alocar matriz\n"); 
        exit(2);
    }

    matConcurrent = (float *) malloc(sizeof(float) * dim * dim);
    if (matConcurrent == NULL) {
        printf("Erro ao alocar matriz\n"); 
        exit(2);
    }

    for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++) {
            mat1[i * dim + j] = randomNumber;
            mat2[i * dim + j] = randomNumber; 
            matSequential[i * dim + j] = 0;
            matConcurrent[i * dim + j] = 0;
        }
    }

    return 0;
}

int *multiplyMatrixsSequentially() {

    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            for(int k = 0; k < dim; k++) {
                matSequential[i * dim + j] += mat1[i * dim + k] * mat2[k * dim + j];
            }
        }
    }

    return 0;
}

void *multiplyMatrixsConcurrently(void *arg) {
    int start, end;
    tArgs *args = (tArgs *) arg;
    start = args->start;
    end = args->end;
    
    for(int i = start; i < end; i++) {
        for(int j = 0; j < dim; j++) {
            for(int k = 0; k < dim; k++) {
                matConcurrent[i * dim + j] += mat1[i * dim + k] * mat2[k * dim + j];
            }
        }
    }

    pthread_exit(NULL);
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