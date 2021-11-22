/* Grade: Concurrent Computing - 2021.2 */
/* Prof.: Silvana Rossetto */
/* Made by.: David R Albuquerque */
/* Lab: 1 */
/* Code: Increment the value of all the elements of an array using two threads. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2
#define ARRAY_SIZE 10000

typedef struct {
    int start, end;
    int *array;
} t_Args;

void initializeArray(); // Funcao utilizada para inicializar o array a ser incrementado
void *incrementArray(void *arg); // Funcao utilizada para incrementar o array de forma multi-thread
int checkArrayIncrement(); // Funaao utilizada para checar se o incremento do array foi realizado corretamente. (single-thread)

// Funcao principal do programa
int main(void) {
    pthread_t tid_sistema[NTHREADS]; // Identificadores das threads no sistema
    int thread; // Variavel auxiliar
    t_Args *arg; // Receberá os argumentos para a thread
    int array[ARRAY_SIZE]; // Array que será incrementado

    initializeArray(array, ARRAY_SIZE);

    for(thread = 0; thread < NTHREADS; thread++) {
        arg = malloc(sizeof(t_Args));

        if (arg == NULL) {
            printf("Erro ao alocar memória para arg. \n"); 
            exit(1);
        }

        arg->start = thread * (ARRAY_SIZE / NTHREADS); // De onde a thread iniciará o calculo, alocando dinamicamente de acordo com a quantidade de threads e tamanho do array.
        arg->end = (thread * (ARRAY_SIZE / NTHREADS)) + (ARRAY_SIZE / NTHREADS); // Onde a thread terminará o calculo, alocando dinamicamente de acordo com a quantidade de threads e tamanho do array.
        arg->array = array;
        
        if (pthread_create(&tid_sistema[thread], NULL, incrementArray, (void*) arg)) {
            printf("Erro ao criar threads para a funcao incrementArray. \n");
            exit(-1);
        } else {
            printf("Thread criada com sucesso. Tid: %ld\n", tid_sistema[thread]);
        }

    }

    // Aguarda as threads finalizarem
    for (thread = 0; thread < NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("Erro ao aguardar as threads terminarem a execução. \n"); exit(-1); 
        } else {
            printf("Thread finalizada com sucesso. Tid: %ld\n", tid_sistema[thread]);
        }
    }

    checkArrayIncrement(array, ARRAY_SIZE);
    printf("Encerrando o programa. O quadrado das posicoes do vetor foi calculado corretamente. \n");

    return 0;
}


void initializeArray(int *array, int size) {
    int i;
    
    for(i = 0; i < size; i++) {
        array[i] = i+1;
    }
}

void *incrementArray (void *arg) {
    t_Args *args = (t_Args *) arg;
    int i;

    for(i = args->start; i < args->end; i++) {
        args->array[i]*=args->array[i];
    }

    free(arg);
    pthread_exit(NULL);
}

int checkArrayIncrement(int *array, int size) {
    int i, square;
    
    for( i = 0; i < size; i++ ){
        square = (i+1) * (i+1);
        if(array[i] != square) {
            printf("Erro ao calcular o quadrado do elemento no array. \n");
            return 1;
        }
    }

    printf("\n");
    return 0;
}
